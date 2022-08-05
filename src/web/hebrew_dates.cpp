#include <ctime>
#include <iostream>
#include <map>
#include <string>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>
#include <nlohmann/json.hpp>

#include "../time_util.h"

#include "hebrew_dates.h"

using nlohmann::json;

HebrewDateMap getHebrewDates(const DateRange& dates) {
    httplib::Client hebcalCli("https://www.hebcal.com");

    auto res = hebcalCli.Get("/converter", httplib::Params(
        {
            { "cfg", "json" },
            { "g2h", "1" },
            { "start", tmToIsoDateString(dates.getStart()) },
            { "end", tmToIsoDateString(dates.getEnd()) }
        }
    ), httplib::Headers());

    if (res->status != 200) throw std::runtime_error("Error getting hebrew dates");

    json::object_t datesJsonObject = json::parse(res->body);

    std::map<tm, std::string, decltype(&lessTmDateOnly)> hebrewDates(&lessTmDateOnly);

    if (datesJsonObject.count("hdates")) {
        datesJsonObject = datesJsonObject["hdates"];

        for (auto [dt, dj] : datesJsonObject) {
            tm gTime = stringToTm(dt);
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

    return hebrewDates;
}