// hh_date.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "date/parse8601.h"
#include "parse8601.h"
#include <iostream>

int main()
{
    using namespace date;
    using namespace std;
    cout << hh::parse8601(istringstream{ "2014-11-12T19:12:14.505Z" }) << '\n';
    cout << hh::parse8601(istringstream{ "2014-11-12T12:12:14.505-5:00" }) << '\n';

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

