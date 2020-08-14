#pragma once

#include <date/date.h>

#include <chrono>
#include <stdexcept>
#include <string_view>

namespace core::time {
enum class iso8601_required {
    YYYY,
    YYYYMM,
    YYYYMMDD,
    YYYYMMDDhh,
    YYYYMMDDhhmm,
    YYYYMMDDhhmmss,
};

template <typename Duration = std::chrono::seconds>
inline std::chrono::time_point<std::chrono::system_clock, Duration> parse_iso8601datetime(std::string_view date, iso8601_required required = iso8601_required::YYYYMMDDhhmmss)
{
    using std::string_view;

    // helper lambdas
    auto is_digit = [](const char ch) { return ch >= '0' && ch <= '9'; };

    auto to_digit = [&is_digit](const char ch)
    {
        if (!is_digit(ch))
            throw std::runtime_error("Not a digit");
        return ch - '0';
    };

    auto integer = [&to_digit, &date](int digits)
    {
        unsigned int number{ 0 };
        while (digits-- > 0)
        {
            if (date.empty())
                throw std::runtime_error("Missing digit");
            number = number * 10 + to_digit(date[0]);
            date.remove_prefix(1);
        }
        return number;
    };

    auto decimal = [&to_digit, &is_digit, &date](int digits)
    {
        unsigned long long number{ 0 };
        unsigned long long divisor{ 1 };
        while (digits-- > 0)
        {
            if (date.empty())
                throw std::runtime_error("Missing digit");
            number = number * 10 + to_digit(date[0]);
            date.remove_prefix(1);
        }
        if (!date.empty() && (date[0] == '.' || date[0] == ','))
        {
            date.remove_prefix(1);
            while (!date.empty() && is_digit(date[0]))
            {
                number = number * 10 + to_digit(date[0]);
                date.remove_prefix(1);
                divisor *= 10;
            }
        }
        return std::make_pair(number, divisor);
    };

    auto process_separator = [&date](int index, bool& has_separator, char separator)
    {
        switch (index)
        {
        case 0:
            has_separator = date[0] == separator;
            break;
        case 1:
            if (has_separator != (date[0] == separator))
                throw std::runtime_error("Separator missing");
            break;
        default:
            throw std::runtime_error("Invalid termination");
        }
        if (has_separator)
            date.remove_prefix(1);
    };

    auto is_leap_year = [](int year)
    {
        return year % 400 == 0 ? true : (year % 100 == 0 ? false : year % 4 == 0);
    };

    auto days_in_month = [&is_leap_year](unsigned int month, int year)
    {
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
            return is_leap_year(year) ? 29u : 28u;
        default:
            assert(false);
            return 0u;
        }
    };

    auto is_positive_sign = [&date]()
    {
        assert(!date.empty());
        if (date[0] == '+')
        {
            date.remove_prefix(1);
            return true;
        }
        if (date[0] == '-')
        {
            date.remove_prefix(1);
            return false;
        }
        // utf-8 minus sign
        if (date.size() > 3 && date.substr(0, 3) == "\xe2\x88\x92")
        {
            date.remove_prefix(3);
            return false;
        }
        throw std::runtime_error("Invalid time offset sign");
    };

    struct timezone_offset_t
    {
        int to_minutes() const noexcept
        {
            int result = hours * 60 + minutes;
            return positive ? result : -result;
        }
        bool positive;
        unsigned int hours;
        unsigned int minutes;
    };

    auto is_valid_timezone_offset = [](const timezone_offset_t& offset)
    {
        switch (offset.minutes)
        {
        case 0:
            return offset.hours <= 12 || (offset.positive && offset.hours <= 14);
        case 30:
            if (offset.positive)
                switch (offset.hours)
                {
                case 3:
                case 4:
                case 5:
                case 6:
                case 9:
                case 10:
                    return true;
                }
            else
                switch (offset.hours)
                {
                case 3:
                case 9:
                    return true;
                }
            return false;
        case 45:
            if (offset.positive)
                switch (offset.hours)
                {
                case 5:
                case 8:
                case 12:
                    return true;
                }
            return false;
        default:
            return false;
        }
    };

    // here starts the actual code!
    union
    {
        unsigned int date_components[3];
        struct
        {
            int year;
            unsigned int month;
            unsigned int day;
        } d{ 0, 1, 1 };
    };

    union
    {
        unsigned int time_components[3];
        struct
        {
            unsigned int hours;
            unsigned int minutes;
            unsigned int seconds;
        } t{ 0, 0, 0 };
    };

    unsigned long long decimals{ 0 };
    timezone_offset_t offset{ true, 0, 0 };

    int parsed{ 0 };

    auto is_end_of_date = [&date]() { return date.empty() || date[0] == 'T'; };
    // read year, month, day
    int digits{ 4 };
    bool has_separator{ false };
    for (int i = 0; i < 3; ++i)
    {
        date_components[i] = integer(digits);

        if (is_end_of_date())
            break;

        ++parsed;
        digits = 2;
        process_separator(i, has_separator, '-');
    }

    if (!date.empty())
    {
        if (date[0] != 'T')
            throw std::runtime_error("Delimiter 'T' is missing");
        date.remove_prefix(1);

        constexpr unsigned long long multipliers[]
        {
            60 * 60 * Duration::period::den,
            60 * Duration::period::den,
            Duration::period::den
        };

        auto is_end_of_time = [&date]()
        {
            return date.empty() || date[0] == 'Z' || date[0] == '+' ||
                date[0] == '-' || date[0] == '\xe2';
        };
        // hours, minutes, seconds
        for (int i = 0; i < 3; ++i)
        {
            auto [digits, divisor] = decimal(2);
            time_components[i] = static_cast<unsigned int>(digits / divisor);
            ++parsed;
            if (divisor != 1)
                decimals = digits % divisor * multipliers[i] / divisor;

            if (is_end_of_time())
                break;

            process_separator(i, has_separator, ':');
        }

        if (!date.empty())
        {
            if (date[0] == 'Z')
                date.remove_prefix(1);
            else {
                // timezone offset
                offset.positive = is_positive_sign();
                offset.hours = integer(2);

                if (!date.empty())
                {
                    if (date[0] != ':')
                        throw std::runtime_error("Missing time offset separator");
                    date.remove_prefix(1);

                    offset.minutes = integer(2);
                }
            }
            if (!date.empty())
                throw std::runtime_error("Invalid termination");
        }
        assert(date.empty());
    }

    if (parsed < static_cast<int>(required))
    {
        if (parsed < static_cast<int>(iso8601_required::YYYYMMDD))
            throw std::runtime_error("Incomplete date");
        throw std::runtime_error("Incomplete time");
    }

    if (d.month < 1 || d.month > 12 || d.day < 1 || d.day > days_in_month(d.month, d.year))
        throw std::runtime_error("Invalid date");

    // 60 seconds is used to denote an added leap second
    // "24:00" may be used for midnight
    if (t.hours > 24 ||
        (t.hours == 24 && (t.minutes != 0 || t.seconds != 0 || decimals != 0)) ||
        t.minutes > 59 || t.seconds > 60 || (t.seconds == 60 && decimals != 0))
        throw std::runtime_error("Invalid time");

    if (!is_valid_timezone_offset(offset))
        throw std::runtime_error("Invalid timezone offset");

    constexpr date::sys_days ref_tp{ date::year{1970} / date::month{1} / date::day{1} };

    long long days =
        (date::sys_days{ date::year{ d.year } / date::month{ d.month } / date::day{ d.day } } -
            ref_tp)
        .count();
    auto count =
        (((days * 24 + t.hours) * 60 + t.minutes - offset.to_minutes()) * 60 +
            t.seconds) *
        Duration::period::den +
        decimals;
    return std::chrono::time_point<std::chrono::system_clock, Duration>{ Duration{ count } };
}
} // namespace core::time
