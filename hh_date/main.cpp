// hh_date.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "date/parse8601.h"
#include "parse8601.h"

#include <chrono>
#include <iostream>


using sys_time = std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>;

inline int number_of_days(int const y1, unsigned int const m1, unsigned int const d1, int const y2, unsigned int const m2, unsigned int const d2)
{
    using namespace date;
    return (sys_days{ year{ y1 } / month{ m1 } / day{ d1 } } - sys_days{ year{ y2 } / month{ m2 } / day{ d2 } }).count();
}

inline sys_time get_systime(int const y1, unsigned int const m1, unsigned int const d1, int const y2, unsigned int const m2, unsigned int const d2)
{
    return sys_time{ std::chrono::seconds{number_of_days(y1, m1, d1, y2, m2, d2) * 24LL * 3600} };
}

int main()
{
    using namespace date;
    using namespace std;

    cout << get_systime(1970, 1, 1, 1970, 1, 1) << '\n';
    cout << get_systime(2200, 1, 1, 1970, 1, 1) << '\n';
    cout << get_systime(1960, 1, 1, 1970, 1, 1) << '\n';


    cout << hh::parse8601(istringstream{ "2014-11-12T19:12:14.505Z" }) << '\n';
    cout << hh::parse8601(istringstream{ "2014-11-12T12:12:14.505-5:00" }) << '\n';
    cout << hh::parse8601(istringstream{ "1900-01-12T12:12:14.505-5:00" }) << '\n';
    cout << hh::parse8601(istringstream{ "1000-01-12T12:12:14.505-5:00" }) << '\n';

    cout << endl;

    cout << core::parse8601("1970-01-01T19:12:14.505Z") << '\n';
    cout << core::parse8601("1971-01-01T19:12:14.505Z") << '\n';
    cout << core::parse8601("1972-01-01T19:12:14.505Z") << '\n';
    cout << core::parse8601("1973-01-01T19:12:14.505Z") << '\n';

    cout << core::parse8601("1999-01-01T19:12:14.505Z") << '\n';
    cout << core::parse8601("2000-01-01T19:12:14.505Z") << '\n';
    cout << core::parse8601("2001-01-01T19:12:14.505Z") << '\n';
    cout << core::parse8601("2002-01-01T19:12:14.505Z") << '\n';
    cout << core::parse8601("2003-01-01T19:12:14.505Z") << '\n';
    cout << core::parse8601("2004-01-01T19:12:14.505Z") << '\n';
    cout << core::parse8601("2005-01-01T19:12:14.505Z") << '\n';

    cout << core::parse8601("2050-01-01T19:12:14.505Z") << '\n';

    cout << core::parse8601("2099-01-01T19:12:14.505Z") << '\n';
    cout << core::parse8601("2100-01-01T19:12:14.505Z") << '\n';
    cout << core::parse8601("2101-01-01T19:12:14.505Z") << '\n';
    cout << core::parse8601("2102-01-01T19:12:14.505Z") << '\n';

    cout << core::parse8601("2199-01-01T19:12:14.505Z") << '\n';
    cout << core::parse8601("2200-01-01T19:12:14.505Z") << '\n';
    cout << core::parse8601("2201-01-01T19:12:14.505Z") << '\n';

    cout << core::parse8601("2399-01-01T19:12:14.505Z") << '\n';
    cout << core::parse8601("2400-01-01T19:12:14.505Z") << '\n';
    cout << core::parse8601("2401-01-01T19:12:14.505Z") << '\n';

    //cout << core::parse8601("2014-11-12T19:12:14.505Z") << '\n';
    //cout << core::parse8601("2014-11-12T12:12:14.505-5:00") << '\n';
}

