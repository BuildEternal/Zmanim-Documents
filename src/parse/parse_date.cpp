#include <ctime>
#include <string>
#include <vector>
#include <iostream>
#include <regex>

#include "../time_util.h"
#include "../invalid_date_exception.h"

using namespace std;

tm parseDate(vector<string>& args)
{
    if (args.size() > 0)
    {
        auto date_str = args[0];
        args.erase(args.begin());

        static const std::regex date_re(R"(^(\d{1,2})/(\d{1,2})/(\d{1,2}|\d{4})$)",
            std::regex::icase);

        smatch date_match;

        bool matched = regex_match(date_str, date_match, date_re);

        if (!matched)
            throw InvalidDateException(date_str);

        int day = stoi(date_match[2]);
        int month = stoi(date_match[1]);
        int year = stoi(date_match[3]);

        if (date_match[3].length() <= 2)
            year += 2000;

        tm date = {
            .tm_sec = 0,
            .tm_min = 0,
            .tm_hour = 0,
            .tm_mday = day,
            .tm_mon = month - 1,
            .tm_year = year - 1900,
            .tm_isdst = -1,
        };

        if (!valid_dmy(date))
            throw InvalidDateException(date_str);

        return date;
    }

    throw InvalidDateException("");
}