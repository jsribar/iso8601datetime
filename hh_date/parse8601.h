#pragma once

#include <cassert>
#include <chrono>
#include <stdexcept>

namespace core
{

using sys_time = std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds>;

sys_time parse8601(const char* date)
{
	auto to_digit = [](char* ch)
	{
		if (*ch < '0' || *ch > '9')
			throw std::runtime_error("Not a digit");
		return *ch - '0';
	};

	auto integer = [&to_digit](char*& ch, int digits)
	{
		int number{ 0 };
		for (int i = 0; i < digits; ++i)
		{
			number = number * 10 + to_digit(ch);
			++ch;
		}
		return number;
	};

	char* ch = const_cast<char*>(date);

	int cur_year = integer(ch, 4);
	if (*ch == '-')
		++ch;

	int cur_month = integer(ch, 2);
	if (*ch == '-')
		++ch;

	int days = integer(ch, 2) - 1;

	if (*ch != 'T')
		throw std::runtime_error("Delimiter 'T' is missing");
	++ch;

	auto decimal = [&to_digit](char*& ch, int digits)
	{
		double number{ 0 };
		for (int i = 0; i < digits; ++i)
		{
			number = number * 10 + to_digit(ch);
			++ch;
		}
		if (*ch != '.' && *ch != ',')
			return number;
		++ch;
		double divisor = 10;
		while (*ch >= '0' && *ch <= '9')
		{
			number += to_digit(ch) / divisor;
			++ch;
			divisor *= 10;
		}
		return number;
	};

	double hours = decimal(ch, 2);
	if (*ch == ':')
		++ch;

	double minutes = decimal(ch, 2);
	if (*ch == ':')
		++ch;

	double seconds = decimal(ch, 2);
	if (*ch == '.')
		++ch;

	// parse offset

	int years = cur_year - 1970;
	int number_of_leap_years = (cur_year - 1969) / 4;
	number_of_leap_years -= (cur_year - 2001) / 100;
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
			return 0;
		}
	};

	int days_in_current_year{ 0 };
	for (int m = 1; m < cur_month; ++m)
		days_in_current_year += days_in_month(cur_month);
	days_in_current_year += days;

	seconds = ((((years * 365. + number_of_leap_years) + days_in_current_year) * 24 + hours) * 60 + minutes) * 60 + seconds;
	return sys_time(std::chrono::milliseconds(static_cast<long long>(seconds * 1000)));
}

}
