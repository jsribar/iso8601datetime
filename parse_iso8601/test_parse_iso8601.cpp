#define CATCH_CONFIG_MAIN

#include "parse_iso8601.h"

#include <catch2/catch.hpp>

#include <sstream>

using std::ostringstream;

using namespace date;
using namespace core::time;

TEST_CASE("parse_iso8601 returns valid date&time for a general date and time format")
{
	// 'YYYY-MM-DDThh:mm:ssZ'
	{
		ostringstream ss;
		ss << parse_iso8601datetime("1970-01-01T23:10:13Z");
		CHECK(ss.str() == "1970-01-01 23:10:13");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime("1900-02-28T23:10:13Z");
		CHECK(ss.str() == "1900-02-28 23:10:13");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime("2120-05-30T23:10:13Z");
		CHECK(ss.str() == "2120-05-30 23:10:13");
	}
	// 'YYYY-MM-DDThh:mm:ss'
	{
		ostringstream ss;
		ss << parse_iso8601datetime("1970-01-01T23:10:13");
		CHECK(ss.str() == "1970-01-01 23:10:13");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime("1900-02-28T23:10:13");
		CHECK(ss.str() == "1900-02-28 23:10:13");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime("2120-05-30T23:10:13");
		CHECK(ss.str() == "2120-05-30 23:10:13");
	}
}

TEST_CASE("parse_iso8601 returns valid date&time for a general date and time format with different time resolutions")
{
	// milliseconds
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("1970-01-01T23:10:13Z");
		CHECK(ss.str() == "1970-01-01 23:10:13.000");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("1900-02-28T23:10:13Z");
		CHECK(ss.str() == "1900-02-28 23:10:13.000");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("2120-05-30T23:10:13Z");
		CHECK(ss.str() == "2120-05-30 23:10:13.000");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("1970-01-01T23:10:13");
		CHECK(ss.str() == "1970-01-01 23:10:13.000");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("1900-02-28T23:10:13");
		CHECK(ss.str() == "1900-02-28 23:10:13.000");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("2120-05-30T23:10:13");
		CHECK(ss.str() == "2120-05-30 23:10:13.000");
	}
	// microseconds
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::microseconds>("2020-08-13T23:10:13.12345Z");
		CHECK(ss.str() == "2020-08-13 23:10:13.123450");
	}
	// nanoseconds
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::nanoseconds>("2020-08-13T23:10:13.123456789Z");
		CHECK(ss.str() == "2020-08-13 23:10:13.123456789");
	}
	// minutes
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::minutes>("2020-01-02T01:00:30.987Z");
		CHECK(ss.str() == "2020-01-02 01:00");
	}
	// hours
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::hours>("2020-01-02T01:30:30Z");
		CHECK(ss.str() == "2020-01-02 0100");
	}
}

TEST_CASE("parse_iso8601 returns valid date&time for a general date and time when required extends components provided")
{
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("2020-08-13T23:10:20Z", iso8601_required::YYYYMMDDhhmm);
		CHECK(ss.str() == "2020-08-13 23:10:20.000");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("2020-08-13T23:10:20", iso8601_required::YYYYMMDDhhmm);
		CHECK(ss.str() == "2020-08-13 23:10:20.000");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("2020-08-13T23:10Z", iso8601_required::YYYYMMDDhh);
		CHECK(ss.str() == "2020-08-13 23:10:00.000");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("2020-08-13T23:10,5Z", iso8601_required::YYYYMMDDhh);
		CHECK(ss.str() == "2020-08-13 23:10:30.000");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("2020-08-13T23:10Z", iso8601_required::YYYYMMDD);
		CHECK(ss.str() == "2020-08-13 23:10:00.000");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("2020-08-13T23:10Z", iso8601_required::YYYYMMDD);
		CHECK(ss.str() == "2020-08-13 23:10:00.000");
	}

	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("1989-10-05T23.5Z", iso8601_required::YYYYMMDD);
		CHECK(ss.str() == "1989-10-05 23:30:00.000");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("1989-10-05T23.50Z", iso8601_required::YYYYMMDD);
		CHECK(ss.str() == "1989-10-05 23:30:00.000");
	}

	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("1989-10-05T23.5Z", iso8601_required::YYYYMM);
		CHECK(ss.str() == "1989-10-05 23:30:00.000");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("1989-10-05T23.50Z", iso8601_required::YYYYMM);
		CHECK(ss.str() == "1989-10-05 23:30:00.000");
	}

	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("1989-10-05T23.5Z", iso8601_required::YYYY);
		CHECK(ss.str() == "1989-10-05 23:30:00.000");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("1989-10-05T23.50Z", iso8601_required::YYYY);
		CHECK(ss.str() == "1989-10-05 23:30:00.000");
	}
}

TEST_CASE("parse_iso8601 returns valid date&time for a general date and time format with decimal seconds")
{
	// 'YYYY-MM-DDThh:mm:ss.sssZ'
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("2020-08-13T23:10:13.123Z");
		CHECK(ss.str() == "2020-08-13 23:10:13.123");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("2020-08-13T23:10:13.12Z");
		CHECK(ss.str() == "2020-08-13 23:10:13.120");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("2020-08-13T23:10:13.1Z");
		CHECK(ss.str() == "2020-08-13 23:10:13.100");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("2020-08-13T23:10:13.12345Z");
		CHECK(ss.str() == "2020-08-13 23:10:13.123");
	}
	// 'YYYY-MM-DDThh:mm:ss.sss'
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("2020-08-13T23:10:13.12345");
		CHECK(ss.str() == "2020-08-13 23:10:13.123");
	}
}

TEST_CASE("parse_iso8601 returns valid date&time for a general date and time with decimal minutes")
{
	// 'YYYY-MM-DDThh:mm.mmmZ'
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("2020-08-13T23:10.5Z", iso8601_required::YYYYMMDDhhmm);
		CHECK(ss.str() == "2020-08-13 23:10:30.000");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("2020-08-13T23:10.50Z", iso8601_required::YYYYMMDDhhmm);
		CHECK(ss.str() == "2020-08-13 23:10:30.000");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("2020-08-13T23:10.500Z", iso8601_required::YYYYMMDDhhmm);
		CHECK(ss.str() == "2020-08-13 23:10:30.000");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("2020-08-13T23:10.5000Z", iso8601_required::YYYYMMDDhhmm);
		CHECK(ss.str() == "2020-08-13 23:10:30.000");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("2020-08-13T23:10.5000", iso8601_required::YYYYMMDDhhmm);
		CHECK(ss.str() == "2020-08-13 23:10:30.000");
	}
}

TEST_CASE("parse_iso8601 returns valid date&time for a general date and time format with decimal hours")
{
	// 'YYYY-MM-DDThh.hhhZ'
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("1989-10-05T23.5Z", iso8601_required::YYYYMMDDhh);
		CHECK(ss.str() == "1989-10-05 23:30:00.000");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("1989-10-05T23.50Z", iso8601_required::YYYYMMDDhh);
		CHECK(ss.str() == "1989-10-05 23:30:00.000");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("1989-10-05T23.500Z", iso8601_required::YYYYMMDDhh);
		CHECK(ss.str() == "1989-10-05 23:30:00.000");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("1989-10-05T23.5000Z", iso8601_required::YYYYMMDDhh);
		CHECK(ss.str() == "1989-10-05 23:30:00.000");
	}
	// 'YYYY-MM-DDThh.hhh'
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("1989-10-05T23.50000", iso8601_required::YYYYMMDDhh);
		CHECK(ss.str() == "1989-10-05 23:30:00.000");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("1989-10-05T23", iso8601_required::YYYYMMDDhh);
		CHECK(ss.str() == "1989-10-05 23:00:00.000");
	}
}

TEST_CASE("parse_iso8601 returns valid date&time for comma decimal separator")
{
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("1993-09-09T23:10:13,123Z");
		CHECK(ss.str() == "1993-09-09 23:10:13.123");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("1993-09-09T23:10:13,12Z");
		CHECK(ss.str() == "1993-09-09 23:10:13.120");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("1993-09-09T23:10:13,1Z");
		CHECK(ss.str() == "1993-09-09 23:10:13.100");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("1993-09-09T23:10:13,12345Z");
		CHECK(ss.str() == "1993-09-09 23:10:13.123");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("1993-09-09T23:10:13,12345");
		CHECK(ss.str() == "1993-09-09 23:10:13.123");
	}

	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("1993-09-09T23:10,5Z", iso8601_required::YYYYMMDDhhmm);
		CHECK(ss.str() == "1993-09-09 23:10:30.000");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("1993-09-09T23:10,50Z", iso8601_required::YYYYMMDDhhmm);
		CHECK(ss.str() == "1993-09-09 23:10:30.000");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("1993-09-09T23:10,500Z", iso8601_required::YYYYMMDDhhmm);
		CHECK(ss.str() == "1993-09-09 23:10:30.000");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("1993-09-09T23:10,5000Z", iso8601_required::YYYYMMDDhhmm);
		CHECK(ss.str() == "1993-09-09 23:10:30.000");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("1993-09-09T23:10,5000", iso8601_required::YYYYMMDDhhmm);
		CHECK(ss.str() == "1993-09-09 23:10:30.000");
	}

	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("1989-10-05T23,5000Z", iso8601_required::YYYYMMDDhh);
		CHECK(ss.str() == "1989-10-05 23:30:00.000");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("1989-10-05T23,5000", iso8601_required::YYYYMMDDhh);
		CHECK(ss.str() == "1989-10-05 23:30:00.000");
	}
}

TEST_CASE("parse_iso8601 returns valid date&time for a general date and time format without separators")
{
	{
		ostringstream ss;
		ss << parse_iso8601datetime("19000228T231013Z");
		CHECK(ss.str() == "1900-02-28 23:10:13");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("19000228T231013,10Z");
		CHECK(ss.str() == "1900-02-28 23:10:13.100");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("19000228T2310Z", iso8601_required::YYYYMMDDhh);
		CHECK(ss.str() == "1900-02-28 23:10:00.000");
	}
}

TEST_CASE("parse_iso8601 returns valid date&time for a general date")
{
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("1970-01-01", iso8601_required::YYYYMMDD);
		CHECK(ss.str() == "1970-01-01 00:00:00.000");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("19700101", iso8601_required::YYYYMMDD);
		CHECK(ss.str() == "1970-01-01 00:00:00.000");
	}

	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("1970-01", iso8601_required::YYYYMM);
		CHECK(ss.str() == "1970-01-01 00:00:00.000");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("197001", iso8601_required::YYYYMM);
		CHECK(ss.str() == "1970-01-01 00:00:00.000");
	}

	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("1970", iso8601_required::YYYY);
		CHECK(ss.str() == "1970-01-01 00:00:00.000");
	}
}

TEST_CASE("parse_iso8601 returns valid date&time for a given timezone offset")
{
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("1970-01-01T22:30:13+04:00");
		CHECK(ss.str() == "1970-01-01 18:30:13.000");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("1970-01-01T22:30:13+04");
		CHECK(ss.str() == "1970-01-01 18:30:13.000");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("1970-01-02T00:00:13+04");
		CHECK(ss.str() == "1970-01-01 20:00:13.000");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("1970-01-01T22:30:13-04");
		CHECK(ss.str() == "1970-01-02 02:30:13.000");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("1900-02-28T20:10:13-03:30");
		CHECK(ss.str() == "1900-02-28 23:40:13.000");
	}
	// minus sign U+2212
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>(u8"1900-02-28T20:10:13\u221203:30");
		CHECK(ss.str() == "1900-02-28 23:40:13.000");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("2120-05-30T23:10:13+00:00");
		CHECK(ss.str() == "2120-05-30 23:10:13.000");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("1970-01-01T15:00:13-03:30");
		CHECK(ss.str() == "1970-01-01 18:30:13.000");
	}
	{
		ostringstream ss;
		ss << parse_iso8601datetime<std::chrono::milliseconds>("1970-01-01T15:00:13-03");
		CHECK(ss.str() == "1970-01-01 18:00:13.000");
	}
}

TEST_CASE("parse_iso8601 throws exception for invalid string")
{
	CHECK_THROWS(parse_iso8601datetime("Z"));
	CHECK_THROWS(parse_iso8601datetime("T"));
	CHECK_THROWS(parse_iso8601datetime("-"));
	CHECK_THROWS(parse_iso8601datetime(":"));
	CHECK_THROWS(parse_iso8601datetime("A"));

	CHECK_THROWS(parse_iso8601datetime("1970.10"));
	CHECK_THROWS(parse_iso8601datetime("1970/10"));
}

TEST_CASE("parse_iso8601 throws exception for incomplete string")
{
	CHECK_THROWS(parse_iso8601datetime("1970-"));
	CHECK_THROWS(parse_iso8601datetime("1970-10-"));
	CHECK_THROWS(parse_iso8601datetime("1970-10-2"));
	CHECK_THROWS(parse_iso8601datetime("1970-10-20T"));
	CHECK_THROWS(parse_iso8601datetime("1970-10-20T2"));
	CHECK_THROWS(parse_iso8601datetime("1970-10-20T22:"));
	CHECK_THROWS(parse_iso8601datetime("1970-10-20T22:3"));
	CHECK_THROWS(parse_iso8601datetime("1970-10-20T22:30:"));
	CHECK_THROWS(parse_iso8601datetime("1970-10-20T22:30:3"));
	CHECK_THROWS(parse_iso8601datetime("1970-10-20T22:30:30+"));
	CHECK_THROWS(parse_iso8601datetime("1970-10-20T22:30:30+2"));

	CHECK_THROWS(parse_iso8601datetime("1970102"));
	CHECK_THROWS(parse_iso8601datetime("19701020T"));
	CHECK_THROWS(parse_iso8601datetime("19701020T2"));
	CHECK_THROWS(parse_iso8601datetime("19701020T223"));
	CHECK_THROWS(parse_iso8601datetime("19701020T22303"));
	CHECK_THROWS(parse_iso8601datetime("19701020T223030+2"));
}

TEST_CASE("parse_iso8601 throws exception if not all required components exist")
{
	CHECK_NOTHROW(parse_iso8601datetime("1970", iso8601_required::YYYY));
	CHECK_THROWS(parse_iso8601datetime("1970", iso8601_required::YYYYMM));

	CHECK_NOTHROW(parse_iso8601datetime("1970-01", iso8601_required::YYYYMM));
	CHECK_THROWS(parse_iso8601datetime("1970-01", iso8601_required::YYYYMMDD));

	CHECK_NOTHROW(parse_iso8601datetime("197001", iso8601_required::YYYYMM));
	CHECK_THROWS(parse_iso8601datetime("197001", iso8601_required::YYYYMMDD));

	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T22", iso8601_required::YYYYMMDDhh));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T22", iso8601_required::YYYYMMDDhhmm));

	CHECK_NOTHROW(parse_iso8601datetime("19700101T22", iso8601_required::YYYYMMDDhh));
	CHECK_THROWS(parse_iso8601datetime("19700101T22", iso8601_required::YYYYMMDDhhmm));

	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T22:13", iso8601_required::YYYYMMDDhhmm));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T22:13", iso8601_required::YYYYMMDDhhmmss));

	CHECK_NOTHROW(parse_iso8601datetime("19700101T2213", iso8601_required::YYYYMMDDhhmm));
	CHECK_THROWS(parse_iso8601datetime("19700101T2213", iso8601_required::YYYYMMDDhhmmss));
}

TEST_CASE("parse_iso8601 throws exception for default required argument if not all components exist")
{
	CHECK_THROWS(parse_iso8601datetime("1970"));
	CHECK_THROWS(parse_iso8601datetime("1970-01"));
	CHECK_THROWS(parse_iso8601datetime("197001"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T22"));
	CHECK_THROWS(parse_iso8601datetime("1970101T22"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T22:13"));
	CHECK_THROWS(parse_iso8601datetime("197001T2213"));
}

TEST_CASE("parse_iso8601 throws exception for invalid number of digits in date or time")
{
	CHECK_THROWS(parse_iso8601datetime("197-01-01T23:10:13Z"));
	CHECK_THROWS(parse_iso8601datetime("1970-1-01T23:10:13Z"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-1T23:10:13Z"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T2:10:13Z"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T02:1:13Z"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T22:10:1"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T22:1"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T2"));
	CHECK_THROWS(parse_iso8601datetime("1970011"));
	CHECK_THROWS(parse_iso8601datetime("19701"));
	CHECK_THROWS(parse_iso8601datetime("1970"));
}

TEST_CASE("parse_iso8601 throws exception for nonconsistent date and time separators")
{
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T231013Z"));
	CHECK_THROWS(parse_iso8601datetime("19700101T23:10:13Z"));

	CHECK_THROWS(parse_iso8601datetime("1970-0101T23:10:13Z"));
	CHECK_THROWS(parse_iso8601datetime("197001-01T23:10:13Z"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:1013Z"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T2310:13Z"));

	CHECK_THROWS(parse_iso8601datetime("1970-0101T23:10:13"));
	CHECK_THROWS(parse_iso8601datetime("197001-01T23:10:13"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:1013"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T2310:13"));
}

TEST_CASE("parse_iso8601 throws exception if date and time are not separated by 'T'")
{
	CHECK_THROWS(parse_iso8601datetime("1970-01-01A23:10:13Z"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01 23:10:13Z"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01A23:10:13"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01 23:10:13"));
}

TEST_CASE("parse_iso8601 throws exception if date and time are not terminated correctly")
{
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:10:13Z "));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:10:13 "));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:10:13A"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:10:13_00:30"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:10:13-00:30Z"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:10:13-00:30.0"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:10:13-01Z"));
}

TEST_CASE("parse_iso8601 throws exception if date is incomplete but time is required")
{
	CHECK_THROWS(parse_iso8601datetime("1970-01T23:10:13Z"));
	CHECK_THROWS(parse_iso8601datetime("197001T23:10:13"));

	CHECK_THROWS(parse_iso8601datetime("1970-01T23:10:13Z", iso8601_required::YYYYMMDD));
	CHECK_THROWS(parse_iso8601datetime("197001T23:10:13", iso8601_required::YYYYMMDD));
}

TEST_CASE("parse_iso8601 throws exception for invalid date or time")
{
	// invalid month value
	CHECK_THROWS(parse_iso8601datetime("1970-13-01T23:10:13"));
	CHECK_THROWS(parse_iso8601datetime("1970-00-01T23:10:13"));
	// invalid day value
	CHECK_THROWS(parse_iso8601datetime("1970-01-35T23:10:13"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-00T23:10:13"));
	// nonexistent date
	CHECK_THROWS(parse_iso8601datetime("1970-02-29T23:10:13"));
	CHECK_THROWS(parse_iso8601datetime("1900-02-29T23:10:13Z"));
	// invalid hours value
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T25:00:00Z"));
	// invalid hours&minutes&seconds combinations
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T24:10:13"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T24:00:13"));
	CHECK_THROWS(parse_iso8601datetime<std::chrono::milliseconds>("1970-01-01T24:00:00.1"));
	// seconds resolution ignores decimals
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T24:00:00.1"));
	// this may be a valid value
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T24:00:00"));
	// invalid minutes
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T22:60:13"));
	// invalid seconds
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T22:30:61"));
	CHECK_THROWS(parse_iso8601datetime<std::chrono::milliseconds>("1970-01-01T22:30:60.1"));
	// this may be a valid value
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T22:30:60"));
}

TEST_CASE("parse_iso8601 throws exception for invalid timezone offset")
{
	// offset too large
	CHECK_THROWS(parse_iso8601datetime("1970-13-01T23:10:13+15:30"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:10:13-13:00"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:10:13+14:30"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:10:13-12:30"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:10:13+15"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:10:13-13"));
	// offset in invalid format
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:10:13+"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:10:13-"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:10:13+1"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:10:13-1"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:10:13+00:60"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:10:13/12:30"));

	// valid offsets
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13-12:00"));
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13-11:00"));
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13-10:00"));
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13-09:30"));
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13-09:00"));
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13-08:00"));
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13-07:00"));
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13-06:00"));
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13-05:00"));
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13-04:00"));
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13-03:30"));
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13-03:00"));
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13-02:00"));
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13-01:00"));
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13-00:00"));

	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13+14:00"));
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13+13:00"));
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13+12:45"));
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13+12:00"));
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13+11:00"));
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13+10:30"));
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13+10:00"));
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13+09:30"));
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13+09:00"));
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13+08:45"));
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13+08:00"));
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13+07:00"));
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13+06:30"));
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13+06:00"));
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13+05:45"));
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13+05:30"));
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13+05:00"));
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13+04:30"));
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13+04:00"));
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13+03:30"));
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13+03:00"));
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13+02:00"));
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13+01:00"));
	CHECK_NOTHROW(parse_iso8601datetime("1970-01-01T23:10:13+00:00"));

	// invalid offsets
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:10:13-12:30"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:10:13-11:30"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:10:13-10:30"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:10:13-08:30"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:10:13-07:30"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:10:13-06:30"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:10:13-05:30"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:10:13-04:30"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:10:13-02:30"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:10:13-01:30"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:10:13-00:30"));

	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:10:13+14:30"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:10:13+13:30"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:10:13+12:30"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:10:13+11:30"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:10:13+08:30"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:10:13+07:30"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:10:13+02:30"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:10:13+01:30"));
	CHECK_THROWS(parse_iso8601datetime("1970-01-01T23:10:13+00:30"));
}
