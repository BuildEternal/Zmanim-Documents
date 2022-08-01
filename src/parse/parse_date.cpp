#include <ctime>
#include <string>
#include <vector>
#include <iostream>
#include <regex>

#include "args.h"
#include "parse_date.h"
#include "time_util.h"
#include "timespan.h"

// TODO: parse phrases like "next shabbos" or "next shavuot"

Timespan parseTimespan(std::vector<std::string>& args) {
    tm beginDate = parseDate(args);

    std::string prep = peakArg(args);

    if (prep == "to" || prep == "through") {
        pullArg(args);

        tm endDate = parseDate(args);

        return Timespan(beginDate, endDate);
    } else {
        return Timespan(beginDate);
    }
}

tm parseDate(std::vector<std::string>& args) {
    auto date_str = pullArg(args);

    if (date_str == "today" || date_str == "now") {
        time_t now = time(NULL);
        return *localtime(&now);
    }
    else if (date_str != "")
    {
        static const std::regex date_re(R"(^(\d{1,2})/(\d{1,2})/(\d{1,2}|\d{4})$)",
            std::regex::icase);

        std::smatch date_match;

        bool matched = std::regex_match(date_str, date_match, date_re);

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