#include <ctime>
#include <string>
#include <vector>
#include <iostream>
#include <regex>

#include "../time_util.h"

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
            throw std::invalid_argument("The date '" + date_str + "' is invalid. Make sure it is a valid date, is "
                "formatted as 'dd/mm/yy' or 'dd/mm/yyyy', and is within a reasonable time frame.");

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
            throw std::invalid_argument("The date '" + date_str + "' is invalid. Make sure it is a valid date, is "
                "formatted as 'dd/mm/yy' or 'dd/mm/yyyy', and is within a reasonable time frame.");

        return date;
    }

    throw std::invalid_argument("No date was entered.");
}