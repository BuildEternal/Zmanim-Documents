#include <ctime>
#include <iostream>
#include <map>
#include <string>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>
#include <nlohmann/json.hpp>

#include "time_util.h"
#include "timespan.h"

#include "get_hebrew_dates.h"

using nlohmann::json;

std::map<tm, std::string, decltype(&lessTmDateOnly)> getHebrewDates(Timespan dates) {
    httplib::Client hebcalCli("https://www.hebcal.com");

    auto res = hebcalCli.Get("/converter", httplib::Params(
        {
            { "cfg", "json" },
            { "g2h", "1" },
            { "start", tmToDateString(dates.getStart()) },
            { "end", tmToDateString(dates.getEnd()) }
        }
    ), httplib::Headers());

    if (res->status != 200) throw std::runtime_error("Error getting hebrew dates");

    json::object_t datesJsonObject = json::parse(res->body);

    std::map<tm, std::string, decltype(&lessTmDateOnly)> hebrewDates(&lessTmDateOnly);

    if (datesJsonObject.count("hdates")) {
        datesJsonObject = datesJsonObject["hdates"];

        for (auto [dt, dj] : datesJsonObject) {
            tm gTime = tmFromString(dt);
            std::string heDate = std::to_string(dj["hd"].get<int>()) + ' ' +
                dj["hm"].get<std::string>() + ", " +
                std::to_string(dj["hy"].get<int>());

            hebrewDates.emplace(gTime, heDate);
        }
    }
    else {
        tm gTime;
        gTime.tm_year = datesJsonObject["gy"].get<int>() - 1900;
        gTime.tm_mon = datesJsonObject["gm"].get<int>() - 1;
        gTime.tm_mday = datesJsonObject["gd"].get<int>();

        std::string heDate = std::to_string(datesJsonObject["hd"].get<int>()) + ' ' +
            datesJsonObject["hm"].get<std::string>() + ", " +
            std::to_string(datesJsonObject["hy"].get<int>());

        hebrewDates.emplace(gTime, heDate);
    }

    for (auto [a, b]: hebrewDates) {
        std::cout << tmToBasicDateString(a) << ' ' << b << '\n';
    }

    return hebrewDates;
}