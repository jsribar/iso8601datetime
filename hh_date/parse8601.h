#pragma once

#include <cassert>
#include <chrono>
#include <stdexcept>

namespace core
{

using sys_time = std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds>;

sys_time parse8601(const char* date)
{
	auto number = [](char*& ch, int count)
	{
		auto digit = [](char* ch)
		{
			if (*ch < '0' || *ch > '9')
				throw std::runtime_error("Not a digit");
			return *ch - '0';
		};

		int number{ 0 };
		for (int i = 0; i < count; ++i)
		{
			number = number * 10 + digit(ch);
			++ch;
		}
		return number;
	};

	char* ch = const_cast<char*>(date);

	int cur_year = number(ch, 4);
	if (*ch == '-')
		++ch;

	int cur_month = number(ch, 2);
	if (*ch == '-')
		++ch;

	int days = number(ch, 2) - 1;

	// parse hours

	// parse minutes

	// parse seconds

	// parse offset

	int years = cur_year - 1970;
	int number_of_leap_years = (cur_year - 1969) / 4;
	if ((cur_year - 1901) % 100 == 0) // && (cur_year - 1601) % 400 != 0)
		number_of_leap_years -= (cur_year - 2001) / 100;
	if ((cur_year - 1601) % 400 != 0)
		number_of_leap_years += (cur_year - 2001) / 400;

	bool is_leap_year = (cur_year % 400 == 0) ? true : (cur_year % 100 == 0 ? false : cur_year % 4 == 0);

	auto days_in_month = [&is_leap_year](int month)
	{
		switch (month)
		{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			return 31;
		case 4:
		case 6:
		case 9:
		case 11:
			return 30;
		case 2:
			return is_leap_year ? 29 : 28;
		default:
			assert(false);
		}
	};

	int days_in_current_year{ 0 };
	for (int m = 1; m < cur_month; ++m)
		days_in_current_year += days_in_month(cur_month);
	days_in_current_year += days;

	long long seconds = ((years * 365LL + number_of_leap_years) + days_in_current_year) * 24 * 60 * 60;
	return sys_time(std::chrono::milliseconds(seconds * 1000));
}

}
