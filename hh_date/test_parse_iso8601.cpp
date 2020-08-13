#define CATCH_CONFIG_MAIN

#include "parse_iso8601.h"

#include <catch2/catch.hpp>

#include <sstream>

using core::time::parse_iso8601;
using std::ostringstream;

using namespace date;

TEST_CASE("parse_iso8601 returns valid date&time for a general date and time format")
{
    // 'YYYY-MM-DDThh:mm:ssZ'
    {
        ostringstream ss;
        ss << parse_iso8601("1970-01-01T23:10:13Z");
        CHECK(ss.str() == "1970-01-01 23:10:13.000");
    }
    {
        ostringstream ss;
        ss << parse_iso8601("1900-02-28T23:10:13Z");
        CHECK(ss.str() == "1900-02-28 23:10:13.000");
    }
    {
        ostringstream ss;
        ss << parse_iso8601("2120-05-30T23:10:13Z");
        CHECK(ss.str() == "2120-05-30 23:10:13.000");
    }

    // 'YYYY-MM-DDThh:mm:ss'
    {
        ostringstream ss;
        ss << parse_iso8601("1970-01-01T23:10:13");
        CHECK(ss.str() == "1970-01-01 23:10:13.000");
    }
    {
        ostringstream ss;
        ss << parse_iso8601("1900-02-28T23:10:13");
        CHECK(ss.str() == "1900-02-28 23:10:13.000");
    }
    {
        ostringstream ss;
        ss << parse_iso8601("2120-05-30T23:10:13");
        CHECK(ss.str() == "2120-05-30 23:10:13.000");
    }
}

TEST_CASE(
    "parse_iso8601 returns valid date&time for a general date and time format with decimal seconds")
{
    // 'YYYY-MM-DDThh:mm:ss.sssZ'
    {
        ostringstream ss;
        ss << parse_iso8601("2020-08-13T23:10:13.123Z");
        CHECK(ss.str() == "2020-08-13 23:10:13.123");
    }
    {
        ostringstream ss;
        ss << parse_iso8601("2020-08-13T23:10:13.12Z");
        CHECK(ss.str() == "2020-08-13 23:10:13.120");
    }
    {
        ostringstream ss;
        ss << parse_iso8601("2020-08-13T23:10:13.1Z");
        CHECK(ss.str() == "2020-08-13 23:10:13.100");
    }
    {
        ostringstream ss;
        ss << parse_iso8601("2020-08-13T23:10:13.12345Z");
        CHECK(ss.str() == "2020-08-13 23:10:13.123");
    }
    // 'YYYY-MM-DDThh:mm:ss.sss'
    {
        ostringstream ss;
        ss << parse_iso8601("2020-08-13T23:10:13.12345");
        CHECK(ss.str() == "2020-08-13 23:10:13.123");
    }
}

TEST_CASE(
    "parse_iso8601 returns valid date&time for a general date and time format with decimal minutes")
{
    // 'YYYY-MM-DDThh:mm.mmmZ'
    {
        ostringstream ss;
        ss << parse_iso8601("2020-08-13T23:10.5Z");
        CHECK(ss.str() == "2020-08-13 23:10:30.000");
    }
    {
        ostringstream ss;
        ss << parse_iso8601("2020-08-13T23:10.50Z");
        CHECK(ss.str() == "2020-08-13 23:10:30.000");
    }
    {
        ostringstream ss;
        ss << parse_iso8601("2020-08-13T23:10.500Z");
        CHECK(ss.str() == "2020-08-13 23:10:30.000");
    }
    {
        ostringstream ss;
        ss << parse_iso8601("2020-08-13T23:10.5000Z");
        CHECK(ss.str() == "2020-08-13 23:10:30.000");
    }
    // 'YYYY-MM-DDThh:mm.mmm'
    {
        ostringstream ss;
        ss << parse_iso8601("2020-08-13T23:10.5000");
        CHECK(ss.str() == "2020-08-13 23:10:30.000");
    }
}

TEST_CASE(
    "parse_iso8601 returns valid date&time for a general date and time format with decimal hours")
{
    // 'YYYY-MM-DDThh.hhhZ'
    {
        ostringstream ss;
        ss << parse_iso8601("1989-10-05T23.5Z");
        CHECK(ss.str() == "1989-10-05 23:30:00.000");
    }
    {
        ostringstream ss;
        ss << parse_iso8601("1989-10-05T23.50Z");
        CHECK(ss.str() == "1989-10-05 23:30:00.000");
    }
    {
        ostringstream ss;
        ss << parse_iso8601("1989-10-05T23.500Z");
        CHECK(ss.str() == "1989-10-05 23:30:00.000");
    }
    {
        ostringstream ss;
        ss << parse_iso8601("1989-10-05T23.5000Z");
        CHECK(ss.str() == "1989-10-05 23:30:00.000");
    }
    // 'YYYY-MM-DDThh.hhh'
    {
        ostringstream ss;
        ss << parse_iso8601("1989-10-05T23.50000");
        CHECK(ss.str() == "1989-10-05 23:30:00.000");
    }
    {
        ostringstream ss;
        ss << parse_iso8601("1989-10-05T23");
        CHECK(ss.str() == "1989-10-05 23:00:00.000");
    }
}

TEST_CASE("parse_iso8601 returns valid date&time for comma decimal separator")
{
    {
        ostringstream ss;
        ss << parse_iso8601("1993-09-09T23:10:13,123Z");
        CHECK(ss.str() == "1993-09-09 23:10:13.123");
    }
    {
        ostringstream ss;
        ss << parse_iso8601("1993-09-09T23:10:13,12Z");
        CHECK(ss.str() == "1993-09-09 23:10:13.120");
    }
    {
        ostringstream ss;
        ss << parse_iso8601("1993-09-09T23:10:13,1Z");
        CHECK(ss.str() == "1993-09-09 23:10:13.100");
    }
    {
        ostringstream ss;
        ss << parse_iso8601("1993-09-09T23:10:13,12345Z");
        CHECK(ss.str() == "1993-09-09 23:10:13.123");
    }
    {
        ostringstream ss;
        ss << parse_iso8601("1993-09-09T23:10:13,12345");
        CHECK(ss.str() == "1993-09-09 23:10:13.123");
    }

    {
        ostringstream ss;
        ss << parse_iso8601("1993-09-09T23:10,5Z");
        CHECK(ss.str() == "1993-09-09 23:10:30.000");
    }
    {
        ostringstream ss;
        ss << parse_iso8601("1993-09-09T23:10,50Z");
        CHECK(ss.str() == "1993-09-09 23:10:30.000");
    }
    {
        ostringstream ss;
        ss << parse_iso8601("1993-09-09T23:10,500Z");
        CHECK(ss.str() == "1993-09-09 23:10:30.000");
    }
    {
        ostringstream ss;
        ss << parse_iso8601("1993-09-09T23:10,5000Z");
        CHECK(ss.str() == "1993-09-09 23:10:30.000");
    }
    {
        ostringstream ss;
        ss << parse_iso8601("1993-09-09T23:10,5000");
        CHECK(ss.str() == "1993-09-09 23:10:30.000");
    }

    {
        ostringstream ss;
        ss << parse_iso8601("1989-10-05T23,5000Z");
        CHECK(ss.str() == "1989-10-05 23:30:00.000");
    }
    {
        ostringstream ss;
        ss << parse_iso8601("1989-10-05T23,5000");
        CHECK(ss.str() == "1989-10-05 23:30:00.000");
    }
}

TEST_CASE(
    "parse_iso8601 returns valid date&time for a general date and time format without separators")
{
    {
        ostringstream ss;
        ss << parse_iso8601("19700101T23:10:13Z");
        CHECK(ss.str() == "1970-01-01 23:10:13.000");
    }
    {
        ostringstream ss;
        ss << parse_iso8601("19000228T231013Z");
        CHECK(ss.str() == "1900-02-28 23:10:13.000");
    }
    {
        ostringstream ss;
        ss << parse_iso8601("21200530T23:10:13Z");
        CHECK(ss.str() == "2120-05-30 23:10:13.000");
    }
}

TEST_CASE("parse_iso8601 returns valid date&time for a general date")
{
    {
        ostringstream ss;
        ss << parse_iso8601("1970-01-01");
        CHECK(ss.str() == "1970-01-01 00:00:00.000");
    }
    {
        ostringstream ss;
        ss << parse_iso8601("19700101");
        CHECK(ss.str() == "1970-01-01 00:00:00.000");
    }

    {
        ostringstream ss;
        ss << parse_iso8601("1970-01");
        CHECK(ss.str() == "1970-01-01 00:00:00.000");
    }
    {
        ostringstream ss;
        ss << parse_iso8601("197001");
        CHECK(ss.str() == "1970-01-01 00:00:00.000");
    }
}

TEST_CASE("parse_iso8601 returns valid date&time for a given timezone offset")
{
    {
        ostringstream ss;
        ss << parse_iso8601("1970-01-01T22:30:13+04:00");
        CHECK(ss.str() == "1970-01-01 18:30:13.000");
    }
    {
        ostringstream ss;
        ss << parse_iso8601("1970-01-01T22:30:13+04");
        CHECK(ss.str() == "1970-01-01 18:30:13.000");
    }
    {
        ostringstream ss;
        ss << parse_iso8601("1970-01-02T00:00:13+04");
        CHECK(ss.str() == "1970-01-01 20:00:13.000");
    }
    {
        ostringstream ss;
        ss << parse_iso8601("1970-01-01T22:30:13-04");
        CHECK(ss.str() == "1970-01-02 02:30:13.000");
    }
    {
        ostringstream ss;
        ss << parse_iso8601("1900-02-28T23:10:13-00:10");
        CHECK(ss.str() == "1900-02-28 23:20:13.000");
    }
    // minus sign U+2212
    {
        ostringstream ss;
        ss << parse_iso8601(u8"1900-02-28T23:10:13\u221200:10");
        CHECK(ss.str() == "1900-02-28 23:20:13.000");
    }
    {
        ostringstream ss;
        ss << parse_iso8601("2120-05-30T23:10:13+00:00");
        CHECK(ss.str() == "2120-05-30 23:10:13.000");
    }
    {
        ostringstream ss;
        ss << parse_iso8601("1970-01-01T15:00:13-03:30");
        CHECK(ss.str() == "1970-01-01 18:30:13.000");
    }
    {
        ostringstream ss;
        ss << parse_iso8601("1970-01-01T15:00:13-03");
        CHECK(ss.str() == "1970-01-01 18:00:13.000");
    }
}

TEST_CASE("parse_iso8601 throws exception for invalid number of digits in date or time")
{
    {
        CHECK_THROWS(parse_iso8601("197-01-01T23:10:13Z"));
    }
    {
        CHECK_THROWS(parse_iso8601("1970-1-01T23:10:13Z"));
    }
    {
        CHECK_THROWS(parse_iso8601("1970-01-1T23:10:13Z"));
    }
    {
        CHECK_THROWS(parse_iso8601("1970-01-01T2:10:13Z"));
    }
    {
        CHECK_THROWS(parse_iso8601("1970-01-01T02:1:13Z"));
    }
    {
        CHECK_THROWS(parse_iso8601("1970-01-01T22:10:1"));
    }
    {
        CHECK_THROWS(parse_iso8601("1970-01-01T22:1"));
    }
    {
        CHECK_THROWS(parse_iso8601("1970-01-01T2"));
    }
    {
        CHECK_THROWS(parse_iso8601("1970011"));
    }
    {
        CHECK_THROWS(parse_iso8601("19701"));
    }
}

TEST_CASE("parse_iso8601 throws exception for nonconsistent date and time separators")
{
    {
        CHECK_THROWS(parse_iso8601("1970-0101T23:10:13Z"));
    }
    {
        CHECK_THROWS(parse_iso8601("197001-01T23:10:13Z"));
    }
    {
        CHECK_THROWS(parse_iso8601("1970-01-01T23:1013Z"));
    }
    {
        CHECK_THROWS(parse_iso8601("1970-01-01T2310:13Z"));
    }

    {
        CHECK_THROWS(parse_iso8601("1970-0101T23:10:13"));
    }
    {
        CHECK_THROWS(parse_iso8601("197001-01T23:10:13"));
    }
    {
        CHECK_THROWS(parse_iso8601("1970-01-01T23:1013"));
    }
    {
        CHECK_THROWS(parse_iso8601("1970-01-01T2310:13"));
    }
}

TEST_CASE("parse_iso8601 throws exception if date and time are not separated by 'T'")
{
    {
        CHECK_THROWS(parse_iso8601("1970-01-01A23:10:13Z"));
    }
    {
        CHECK_THROWS(parse_iso8601("1970-01-01 23:10:13Z"));
    }
    {
        CHECK_THROWS(parse_iso8601("1970-01-01A23:10:13"));
    }
    {
        CHECK_THROWS(parse_iso8601("1970-01-01 23:10:13"));
    }
}

TEST_CASE("parse_iso8601 throws exception if date and time are not terminated correctly")
{
    {
        CHECK_THROWS(parse_iso8601("1970-01-01T23:10:13Z "));
    }
    {
        CHECK_THROWS(parse_iso8601("1970-01-01T23:10:13 "));
    }
    {
        CHECK_THROWS(parse_iso8601("1970-01-01T23:10:13A"));
    }
    {
        CHECK_THROWS(parse_iso8601("1970-01-01T23:10:13_00:30"));
    }
    {
        CHECK_THROWS(parse_iso8601("1970-01-01T23:10:13-00:30Z"));
    }
    {
        CHECK_THROWS(parse_iso8601("1970-01-01T23:10:13-00:30.0"));
    }
    {
        CHECK_THROWS(parse_iso8601("1970-01-01T23:10:13-01Z"));
    }
}

TEST_CASE("parse_iso8601 throws exception for invalid date")
{
    {
        CHECK_THROWS(parse_iso8601("1970-13-01T23:10:13_00:30"));
    }
    {
        CHECK_THROWS(parse_iso8601("1970-00-01T23:10:13_00:30"));
    }
    {
        CHECK_THROWS(parse_iso8601("1970-01-35T23:10:13"));
    }
    {
        CHECK_THROWS(parse_iso8601("1970-01-00T23:10:13"));
    }
    {
        CHECK_THROWS(parse_iso8601("1970-02-29T23:10:13"));
    }
    {
        CHECK_THROWS(parse_iso8601("2000-02-29T23:10:13-00:30Z"));
    }
}

TEST_CASE("parse_iso8601 throws exception for invalid time offset")
{
    {
        CHECK_THROWS(parse_iso8601("1970-13-01T23:10:13+15:30"));
    }
    {
        CHECK_THROWS(parse_iso8601("1970-00-01T23:10:13+00:60"));
    }
    {
        CHECK_THROWS(parse_iso8601("1970-01-35T23:10:13+14:30"));
    }
    {
        CHECK_THROWS(parse_iso8601("1970-01-35T23:10:13-13:00"));
    }
    {
        CHECK_THROWS(parse_iso8601("1970-01-35T23:10:13-12:30"));
    }
    {
        CHECK_THROWS(parse_iso8601("1970-01-35T23:10:13/12:30"));
    }

    {
        CHECK_THROWS(parse_iso8601("1970-13-01T23:10:13+15"));
    }
    {
        CHECK_THROWS(parse_iso8601("1970-01-35T23:10:13-13"));
    }
}
