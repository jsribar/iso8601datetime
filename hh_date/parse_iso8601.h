#pragma once

#include <date/date.h>

#include <chrono>
#include <stdexcept>

namespace core::time {

    using sys_time = std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds>;

    inline sys_time parse_iso8601(const char* date)
    {
        // helper lambdas
        auto is_digit = [](const char* ch) { return *ch >= '0' && *ch <= '9'; };

        auto to_digit = [&is_digit](const char* ch) {
            if (!is_digit(ch))
                throw std::runtime_error("Not a digit");
            return *ch - '0';
        };

        auto integer = [&to_digit](char*& ch, int digits) {
            unsigned int number{ 0 };
            for (int i = 0; i < digits; ++i)
            {
                number = number * 10 + to_digit(ch);
                ++ch;
            }
            return number;
        };

        auto decimal = [&to_digit, &is_digit](char*& ch, int digits) {
            int number{ 0 };
            int divisor{ 1 };
            for (int i = 0; i < digits; ++i)
            {
                number = number * 10 + to_digit(ch);
                ++ch;
            }
            if (*ch == '.' || *ch == ',')
            {
                ++ch;
                while (is_digit(ch))
                {
                    number = number * 10 + to_digit(ch);
                    ++ch;
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

        auto is_positive_sign = [](char*& ch) {
            if (*ch == '+')
            {
                ++ch;
                return true;
            }
            if (*ch == '-')
            {
                ++ch;
                return false;
            }
            // utf-8 minus sign
            if (*ch++ == '\xe2' && *ch++ == '\x88' && *ch++ == '\x92')
                return false;
            throw std::runtime_error("Invalid time offset sign");
        };

        // here starts the actual code
        char* ch = const_cast<char*>(date);

        int year = integer(ch, 4);

        bool has_date_separator = *ch == '-';
        if (has_date_separator)
            ++ch;

        unsigned int month = integer(ch, 2);
        if (month < 1 || month > 12)
            throw std::runtime_error("Invalid date");

        unsigned int day{ 1 };
        unsigned int hours{ 0 };
        unsigned int minutes{ 0 };
        unsigned int seconds{ 0 };
        unsigned int milliseconds{ 0 };
        int          time_offset{ 0 };

        if (*ch != '\0')
        {
            if (has_date_separator)
            {
                if (*ch != '-')
                    throw std::runtime_error("Date separator missing");
                ++ch;
            }

            day = integer(ch, 2);

            bool is_leap_year = year % 400 == 0 ? true : (year % 100 == 0 ? false : year % 4 == 0);
            if (day < 1 || day > days_in_month(month, is_leap_year))
                throw std::runtime_error("Invalid date");

            if (*ch != '\0')
            {
                if (*ch != 'T')
                    throw std::runtime_error("Delimiter 'T' is missing");
                ++ch;

                auto [digits, divisor] = decimal(ch, 2);
                hours = digits / divisor;
                if (hours > 24)
                    throw std::runtime_error("Invalid time");
                if (divisor != 1)
                    milliseconds = digits % divisor * 60 * 60 / divisor * 1000;
                else if (*ch != '\0')
                {
                    bool has_time_separator = *ch == ':';
                    if (has_time_separator)
                        ++ch;

                    auto [digits, divisor] = decimal(ch, 2);
                    minutes = digits / divisor;
                    if (minutes > 60 || (hours == 24 && minutes != 0))
                        throw std::runtime_error("Invalid time");
                    if (divisor != 1)
                        milliseconds = digits % divisor * 60 * 1000 / divisor;
                    else
                    {
                        if (has_time_separator)
                        {
                            if (*ch != ':')
                                throw std::runtime_error("Time separator missing");
                            ++ch;
                        }

                        auto [digits, divisor] = decimal(ch, 2);
                        seconds = digits / divisor;
                        if (divisor != 1)
                            milliseconds = digits % divisor * 1000 / divisor;
                    }
                }
                if (*ch != '\0')
                {
                    if (*ch == 'Z')
                        ++ch;
                    else
                    {
                        // time offset
                        bool is_offset_positive = is_positive_sign(ch);
                        auto hours_offset = integer(ch, 2);
                        if ((is_offset_positive && hours_offset > 14) || hours_offset > 12)
                            throw std::runtime_error("Invalid time offset");

                        time_offset = hours_offset * 60;

                        if (*ch != '\0')
                        {
                            if (*ch != ':')
                                throw std::runtime_error("Missing time offset separator");
                            ++ch;

                            auto minutes_offset = integer(ch, 2);
                            if (minutes_offset >= 60)
                                throw std::runtime_error("Invalid time offset");
                            time_offset += minutes_offset;
                        }
                        if (is_offset_positive)
                            time_offset = -time_offset;
                    }
                    if (*ch != '\0')
                        throw std::runtime_error("Invalid termination");
                }
                assert(*ch == '\0');
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
