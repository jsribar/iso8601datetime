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
inline std::chrono::time_point<std::chrono::system_clock, Duration>
parse_iso8601datetime(
    std::string_view date,
    iso8601_required required = iso8601_required::YYYYMMDDhhmmss) {
  using std::string_view;

  // helper lambdas
  auto is_digit = [](const char ch) { return ch >= '0' && ch <= '9'; };

  auto to_digit = [&is_digit](const char ch) {
    if (!is_digit(ch))
      throw std::runtime_error("Not a digit");
    return ch - '0';
  };

  auto integer = [&to_digit](string_view &view, int digits) {
    unsigned int number{0};
    while (digits-- > 0) {
      if (view.empty())
        throw std::runtime_error("Missing digit");
      number = number * 10 + to_digit(view[0]);
      view.remove_prefix(1);
    }
    return number;
  };

  auto decimal = [&to_digit, &is_digit](string_view &view, int digits) {
    unsigned long long number{0};
    unsigned long long divisor{1};
    while (digits-- > 0) {
      if (view.empty())
        throw std::runtime_error("Missing digit");
      number = number * 10 + to_digit(view[0]);
      view.remove_prefix(1);
    }
    if (!view.empty() && (view[0] == '.' || view[0] == ',')) {
      view.remove_prefix(1);
      while (!view.empty() && is_digit(view[0])) {
        number = number * 10 + to_digit(view[0]);
        view.remove_prefix(1);
        divisor *= 10;
      }
    }
    return std::make_pair(number, divisor);
  };

  auto is_leap_year = [](int year) {
    return year % 400 == 0 ? true : (year % 100 == 0 ? false : year % 4 == 0);
  };

  auto days_in_month = [&is_leap_year](unsigned int month, int year) {
    switch (month) {
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

  auto is_positive_sign = [](string_view &view) {
    assert(!view.empty());
    if (view[0] == '+') {
      view.remove_prefix(1);
      return true;
    }
    if (view[0] == '-') {
      view.remove_prefix(1);
      return false;
    }
    // utf-8 minus sign
    if (view.size() > 3 && view.substr(0, 3) == "\xe2\x88\x92") {
      view.remove_prefix(3);
      return false;
    }
    throw std::runtime_error("Invalid time offset sign");
  };

  struct timezone_offset_t {
    int to_minutes() {
      int result = hours * 60 + minutes;
      return positive ? result : -result;
    }
    bool positive;
    unsigned int hours;
    unsigned int minutes;
  };

  auto is_valid_timezone_offset = [](const timezone_offset_t &offset) {
    switch (offset.minutes) {
    case 0:
      return offset.hours <= 12 || (offset.positive && offset.hours <= 14);
    case 30:
      if (offset.positive)
        switch (offset.hours) {
        case 3:
        case 4:
        case 5:
        case 6:
        case 9:
        case 10:
          return true;
        }
      else
        switch (offset.hours) {
        case 3:
        case 9:
          return true;
        }
      return false;
    case 45:
      if (offset.positive)
        switch (offset.hours) {
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

  // here starts the actual code
  unsigned int month{1};
  unsigned int day{1};

  union {
    unsigned int time_components[3];
    struct {
      unsigned int hours;
      unsigned int minutes;
      unsigned int seconds;
    } t{0, 0, 0};
  };

  unsigned long long decimals{0};
  timezone_offset_t offset{true, 0, 0};

  int year = integer(date, 4);
  int parsed{0};

  if (!date.empty()) {
    bool has_date_separator = date[0] == '-';
    if (has_date_separator)
      date.remove_prefix(1);

    month = integer(date, 2);
    ++parsed;

    if (!date.empty()) {
      if (has_date_separator) {
        if (date[0] != '-')
          throw std::runtime_error("Date separator missing");
        date.remove_prefix(1);
      }

      day = integer(date, 2);
      ++parsed;

      if (!date.empty()) {
        if (date[0] != 'T')
          throw std::runtime_error("Delimiter 'T' is missing");
        date.remove_prefix(1);

        constexpr unsigned long long multipliers[]{
            60 * 60 * Duration::period::den, 60 * Duration::period::den,
            Duration::period::den};

        bool has_time_separator{false};

        auto is_end_of_time = [&date]() {
          return date.empty() || date[0] == 'Z' || date[0] == '+' ||
                 date[0] == '-' || date[0] == '\xe2';
        };

        for (int i = 0; i < 3; ++i) {
          auto [digits, divisor] = decimal(date, 2);
          time_components[i] = static_cast<unsigned int>(digits / divisor);
          ++parsed;
          if (divisor != 1)
            decimals = digits % divisor * multipliers[i] / divisor;

          if (is_end_of_time())
            break;

          switch (i) {
          case 0:
            has_time_separator = date[0] == ':';
            break;
          case 1:
            if (has_time_separator != (date[0] == ':'))
              throw std::runtime_error("Time separator missing");
            break;
          default:
            throw std::runtime_error("Invalid termination");
          }
          if (has_time_separator)
            date.remove_prefix(1);
        }

        if (!date.empty()) {
          if (date[0] == 'Z')
            date.remove_prefix(1);
          else {
            // timezone offset
            offset.positive = is_positive_sign(date);
            offset.hours = integer(date, 2);

            if (!date.empty()) {
              if (date[0] != ':')
                throw std::runtime_error("Missing time offset separator");
              date.remove_prefix(1);

              offset.minutes = integer(date, 2);
            }
          }
          if (!date.empty())
            throw std::runtime_error("Invalid termination");
        }
        assert(date.empty());
      }
    }
  }

  if (parsed < static_cast<int>(required)) {
    if (parsed < static_cast<int>(iso8601_required::YYYYMMDD))
      throw std::runtime_error("Incomplete date");
    throw std::runtime_error("Incomplete time");
  }

  if (month < 1 || month > 12 || day < 1 || day > days_in_month(month, year))
    throw std::runtime_error("Invalid date");

  // 60 seconds is used to denote an added leap second
  // "24:00" may be used for midnight
  if (t.hours > 24 ||
      (t.hours == 24 && (t.minutes != 0 || t.seconds != 0 || decimals != 0)) ||
      t.minutes > 59 || t.seconds > 60 || (t.seconds == 60 && decimals != 0))
    throw std::runtime_error("Invalid time");

  if (!is_valid_timezone_offset(offset))
    throw std::runtime_error("Invalid timezone offset");

  constexpr date::sys_days ref_tp{date::year{1970} / date::month{1} /
                                  date::day{1}};

  long long days =
      (date::sys_days{date::year{year} / date::month{month} / date::day{day}} -
       ref_tp)
          .count();
  auto count =
      (((days * 24 + t.hours) * 60 + t.minutes - offset.to_minutes()) * 60 +
       t.seconds) *
          Duration::period::den +
      decimals;
  return std::chrono::time_point<std::chrono::system_clock, Duration>{
      Duration{count}};
}

} // namespace core::time
