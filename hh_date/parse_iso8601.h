#pragma once

#include <date/date.h>

#include <chrono>
#include <stdexcept>
#include <string_view>

namespace core::time {

	using sys_time = std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds>;

	inline sys_time parse_iso8601datetime(std::string_view date)
	{
		using ch_it = std::string_view::iterator;
		using std::string_view;

		// helper lambdas
		auto is_digit = [](const char ch) { return ch >= '0' && ch <= '9'; };

		auto to_digit = [&is_digit](const char ch) {
			if (!is_digit(ch))
				throw std::runtime_error("Not a digit");
			return ch - '0';
		};

		auto integer = [&to_digit](string_view& view, int digits) {
			unsigned int number{ 0 };
			while (digits-- > 0)
			{
				if (view.empty())
					throw std::runtime_error("Missing digit");
				number = number * 10 + to_digit(view[0]);
				view.remove_prefix(1);
			}
			return number;
		};

		auto decimal = [&to_digit, &is_digit](string_view& view, int digits) {
			int number{ 0 };
			int divisor{ 1 };
			while (digits-- > 0)
			{
				if (view.empty())
					throw std::runtime_error("Missing digit");
				number = number * 10 + to_digit(view[0]);
				view.remove_prefix(1);
			}
			if (!view.empty() && (view[0] == '.' || view[0] == ','))
			{
				view.remove_prefix(1);
				while (!view.empty() && is_digit(view[0]))
				{
					number = number * 10 + to_digit(view[0]);
					view.remove_prefix(1);
					divisor *= 10;
				}
			}
			return std::make_pair(number, divisor);
		};

		auto days_in_month = [](const unsigned int month, bool is_leap_year) {
			switch (month)
			{
			case 4:
			case 6:
			case 9:
			case 11:
				return 30u;
			case 1:
			case 3:
			case 5:
			case 7:
			case 8:
			case 10:
			case 12:
				return 31u;
			case 2:
				return is_leap_year ? 29u : 28u;
			default:
				assert(false);
				return 0u;
			}
		};

		auto is_positive_sign = [](string_view& view) {
			assert(!view.empty());
			if (view[0] == '+')
			{
				view.remove_prefix(1);
				return true;
			}
			if (view[0] == '-')
			{
				view.remove_prefix(1);
				return false;
			}
			// utf-8 minus sign
			if (view.size() > 3 && view.substr(0, 3) == "\xe2\x88\x92")
			{
				view.remove_prefix(3);
				return false;
			}
			throw std::runtime_error("Invalid time offset sign");
		};

		// here starts the actual code
		int year = integer(date, 4);

		if (date.empty())
			throw std::runtime_error("Invalid date");

		bool has_date_separator = date[0] == '-';
		if (has_date_separator)
			date.remove_prefix(1);

		unsigned int month = integer(date, 2);
		if (month < 1 || month > 12)
			throw std::runtime_error("Invalid date");

		unsigned int day{ 1 };
		unsigned int hours{ 0 };
		unsigned int minutes{ 0 };
		unsigned int seconds{ 0 };
		unsigned int milliseconds{ 0 };
		int          time_offset{ 0 };

		if (!date.empty())
		{
			if (has_date_separator)
			{
				if (date[0] != '-')
					throw std::runtime_error("Date separator missing");
				date.remove_prefix(1);
			}

			day = integer(date, 2);

			bool is_leap_year = year % 400 == 0 ? true : (year % 100 == 0 ? false : year % 4 == 0);
			if (day < 1 || day > days_in_month(month, is_leap_year))
				throw std::runtime_error("Invalid date");

			if (!date.empty())
			{
				if (date[0] != 'T')
					throw std::runtime_error("Delimiter 'T' is missing");
				date.remove_prefix(1);;

				auto [digits, divisor] = decimal(date, 2);
				hours = digits / divisor;
				if (hours > 24)
					throw std::runtime_error("Invalid time");
				if (divisor != 1)
					milliseconds = digits % divisor * 60 * 60 / divisor * 1000;
				else if (!date.empty())
				{
					bool has_time_separator = date[0] == ':';
					if (has_time_separator)
						date.remove_prefix(1);;

					auto [digits, divisor] = decimal(date, 2);
					minutes = digits / divisor;
					if (minutes > 60 || (hours == 24 && minutes != 0))
						throw std::runtime_error("Invalid time");
					if (divisor != 1)
						milliseconds = digits % divisor * 60 * 1000 / divisor;
					else
					{
						if (has_time_separator)
						{
							if (date[0] != ':')
								throw std::runtime_error("Time separator missing");
							date.remove_prefix(1);;
						}

						auto [digits, divisor] = decimal(date, 2);
						seconds = digits / divisor;
						if (divisor != 1)
							milliseconds = digits % divisor * 1000 / divisor;
					}
				}
				if (!date.empty())
				{
					if (date[0] == 'Z')
						date.remove_prefix(1);
					else
					{
						// time offset
						bool is_offset_positive = is_positive_sign(date);
						auto hours_offset = integer(date, 2);
						if ((is_offset_positive && hours_offset > 14) || hours_offset > 12)
							throw std::runtime_error("Invalid time offset");

						time_offset = hours_offset * 60;

						if (!date.empty())
						{
							if (date[0] != ':')
								throw std::runtime_error("Missing time offset separator");
							date.remove_prefix(1);;

							auto minutes_offset = integer(date, 2);
							if (minutes_offset >= 60)
								throw std::runtime_error("Invalid time offset");
							time_offset += minutes_offset;
						}
						if (is_offset_positive)
							time_offset = -time_offset;
					}
					if (!date.empty())
						throw std::runtime_error("Invalid termination");
				}
				assert(date.empty());
			}
		}

		constexpr date::sys_days ref_timepoint{ date::year{ 1970 } / date::month{ 1 } /
												date::day{ 1 } };

		const auto number_of_days =
			(date::sys_days{ date::year{ year } / date::month{ month } / date::day{ day } } -
				ref_timepoint)
			.count();
		return sys_time{ std::chrono::milliseconds(
			(((static_cast<long long>(number_of_days) * 24 + hours) * 60 + minutes + time_offset) * 60 +
			 seconds) *
				1000 +
			milliseconds) };
	}

} // namespace core::time
