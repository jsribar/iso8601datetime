#pragma once

#include <date/date.h>
#include <iostream>
#include <sstream>

namespace hh
{

date::sys_time<std::chrono::milliseconds>
parse8601(std::istream&& is)
{
    std::string save;
    is >> save;
    std::istringstream in{ save };
    date::sys_time<std::chrono::milliseconds> tp;
    in >> date::parse("%FT%TZ", tp);
    if (in.fail())
    {
        in.clear();
        in.exceptions(std::ios::failbit);
        in.str(save);
        in >> date::parse("%FT%T%Ez", tp);
    }
    return tp;
}

}

