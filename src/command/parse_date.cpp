#include <ctime>
#include <string>
#include <vector>
#include <iostream>
#include <regex>

#include "args_util.h"
#include "parse_date.h"
#include "time_util.h"

// TODO: parse phrases like "next shabbos" or "next shavuot"

DateRange parseDateRange(std::vector<std::string>& args) {
    std::string keyword = peakArg(args);

    if (keyword == "next" || keyword == "this") {
        pullArg(args);

        std::string word = pullArg(args);

        if (word == "shabbos" || word == "shabbat") {
            time_t currentTime = time(nullptr);
            tm currentDate = *localtime(&currentTime);

            int daysUntilShabbos = 6 - currentDate.tm_wday;

            tm shabbos = currentDate;
            shabbos.tm_mday += daysUntilShabbos;
            shabbos = normalizeTm(shabbos);

            tm erevShabbos = currentDate;
            erevShabbos.tm_mday += daysUntilShabbos - 1;
            erevShabbos = normalizeTm(erevShabbos);

            return DateRange(erevShabbos, shabbos);
        }
        else throw std::invalid_argument("The meaning of '" + word + "' after 'next' is unknown.");
    }
    else {
        tm beginDate = parseDate(args);

        std::string prep = peakArg(args);

        if (prep == "to" || prep == "through") {
            pullArg(args);

            tm endDate = parseDate(args);

            return DateRange(beginDate, endDate);
        }
        else {
            return DateRange(beginDate);
        }
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
            throw std::invalid_argument("The date '" + date_str + "' is invalid.");

        int day = stoi(date_match[2]);
        int month = stoi(date_match[1]);
        int year = stoi(date_match[3]);

        if (date_match[3].length() <= 2)
            year += 2000;

        tm date = {
            .tm_mday = day,
            .tm_mon = month - 1,
            .tm_year = year - 1900,
        };

        if (!validTmDateOnly(date))
            throw std::invalid_argument("The date '" + date_str + "' is invalid.");

        return date;
    }

    throw std::invalid_argument("No date was entered.");
}