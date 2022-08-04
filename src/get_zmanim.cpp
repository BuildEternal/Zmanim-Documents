#include <iostream>
#include <ctime>
#include <regex>
#include <string>
#include <sstream>
#include <iomanip>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>
#include <nlohmann/json.hpp>

#include "forecast.h"
#include "get_hebrew_dates.h"
#include "time_util.h"
#include "timespan.h"
#include "html_builders/day_info.h"
#include "html_builders/document_info.h"
#include "html_builders/weather_entry_info.h"
#include "html_builders/zman_info.h"

using json = nlohmann::json;

// void parseJsonData(
//     std::map<tm, std::map<std::string, tm>, decltype(&lessTm)>& data,
//     const std::string& zmanDateStr,
//     const std::string& zmanTimeStr,
//     const std::string& zmanIdStr
// ) {
//     std::regex dateRegex(R"((\d{4})-(\d{2})-(\d{2}))");
//     std::smatch dateVals;
//     std::regex_search(zmanDateStr, dateVals, dateRegex);

//     tm zmanDateTm = {
//         .tm_mday = std::stoi(dateVals[3]),
//         .tm_mon = std::stoi(dateVals[2]) - 1,
//         .tm_year = std::stoi(dateVals[1]) - 1900,
//     };

//     std::regex timeRegex(R"((\d{4})-(\d{2})-(\d{2})T(\d{2}):(\d{2}):(?:\d{2}))");
//     std::smatch timeVals;
//     std::regex_search(zmanTimeStr, timeVals, timeRegex);

//     tm zmanTimeTm = {
//         .tm_min = std::stoi(timeVals[5]),
//         .tm_hour = std::stoi(timeVals[4]),
//         .tm_mday = std::stoi(timeVals[3]),
//         .tm_mon = std::stoi(timeVals[2]) - 1,
//         .tm_year = std::stoi(timeVals[1]) - 1900,
//     };

//     data[zmanDateTm][zmanIdStr] = zmanTimeTm;
// }

/**
 * @brief Gets the zmanim for the given timespan.
 *
 * @param timespan The zmanim range
 * @return The zmanim JSON data
 */
DocumentInfo get_zmanim(
    const Timespan& timespan, std::string locationType, std::string locationId,
    double forecastLat, double forecastLong
) {
    // Get zmanim

    httplib::Client cli("https://www.chabad.org");

    tm start = timespan.getStart();
    tm end = timespan.getEnd();

    auto res = cli.Get("/webservices/zmanim/zmanim/Get_Zmanim", httplib::Params(
        {
            { "startdate", tmToBasicDateString(start) },
            { "enddate", tmToBasicDateString(end) },
            { "locationtype", locationType },
            { "locationid", locationId }
        }
    ), httplib::Headers(
        { { "Accept", "application/json" } }
    ));

    const std::string getZmanimError = "An error accurred when getting zmanim.";

    if (res->status != 200) throw std::runtime_error(getZmanimError);

    auto zmanimJson = json::parse(res->body);

    if (zmanimJson["LocationId"].is_null()) throw std::runtime_error(getZmanimError);

    // Get weather

    auto forecast = getForecastForCoords(forecastLat, forecastLong);

    // Get Hebrew dates

    auto heDates = getHebrewDates(timespan);

    // Extract data from JSON

    std::vector<DayInfo> days;

    json daysJsonArray = zmanimJson["Days"];

    for (auto i = daysJsonArray.begin(); i < daysJsonArray.end(); i++) {
        json dayJson = *i;
        auto daysIndex = i - daysJsonArray.begin();

        tm date = tmFromString(dayJson["DisplayDate"].get<std::string>(), true);

        date.tm_wday = dayJson["DayOfWeek"].get<int>();

        std::string heDate = heDates[date];

        std::string holiday;

        {
            json holidayJson = dayJson["HolidayName"];

            if (holidayJson.is_string())
                holiday = holidayJson.get<std::string>();
        }

        std::string parsha;

        {
            json parshaJson = dayJson["Parsha"];

            if (parshaJson.type() == json::value_t::string)
                parsha = parshaJson.get<std::string>();
        }

        std::vector<ZmanInfo> zmanim;

        json zmanimJsonArray = dayJson["TimeGroups"];

        for (auto j = zmanimJsonArray.begin(); j < zmanimJsonArray.end(); j++) {
            json zmanJson = *j;

            std::string title = zmanJson["Title"].get<std::string>();
            std::string zman = zmanJson["Items"][0]["Zman"].get<std::string>();

            ZmanInfo zmanInfo(title, zman);

            zmanim.push_back(zmanInfo);
        }

        WeatherEntryInfo dayForecast = forecast[date][true];
        WeatherEntryInfo nightForecast = forecast[date][false];

        DayInfo dayInfo(date, heDate, holiday, parsha, zmanim, dayForecast, nightForecast);

        days.push_back(dayInfo);
    }

    std::string location = zmanimJson["LocationName"].get<std::string>();

    std::string locationDetails = zmanimJson["LocationDetails"].get<std::string>();

    std::string returnedLocationId = zmanimJson["LocationId"].get<std::string>();

    DocumentInfo documentInfo(days, location, locationDetails, returnedLocationId);

    return documentInfo;

    // httplib::Client cli("https://www.hebcal.com");

    // tm startDate = timespan.getStart();
    // tm endDate = timespan.getEnd();

    // std::string startDateStr = (std::stringstream() << std::setfill('0') <<
    //     std::setw(4) << (startDate.tm_year + 1900) << '-' <<
    //     std::setw(2) << (startDate.tm_mon + 1) << '-' <<
    //     std::setw(2) << startDate.tm_mday).str();
    // std::string endDateStr = (std::stringstream() << std::setfill('0') <<
    //     std::setw(4) << (endDate.tm_year + 1900) << '-' <<
    //     std::setw(2) << (endDate.tm_mon + 1) << '-' <<
    //     std::setw(2) << endDate.tm_mday).str();

    // httplib::Params params;
    // params.emplace("cfg", "json");
    // params.emplace("start", startDateStr);
    // params.emplace("end", endDateStr);
    // if (locationType == "geoname") params.emplace("geonameid", locationId);
    // else if (locationType == "zip") params.emplace("zip", locationId);
    // else throw std::runtime_error("Unknown location type");

    // auto res = cli.Get("/zmanim", params, httplib::Headers());
    // // std::cout << res->body << '\n';

    // if (res->status != 200) throw std::runtime_error("An error occured while getting zmanim.");

    // json resJson = json::parse(res->body);

    // json dateJson = resJson["date"];
    // json timesJson = resJson["times"];

    // std::cout << dateJson.dump(4) << '\n';

    // // First tm is only date, second tm is only time.
    // std::map<tm, std::map<std::string, tm>, decltype(&lessTm)> zmanimData(&lessTm);

    // if (dateJson.is_object()) {
    //     for (auto& [zmanIdStr, zmanDatesJson] : timesJson.items()) {
    //         for (auto& [zmanDateStr, zmanTimeJson] : zmanDatesJson.items()) {
    //             std::string zmanTimeStr = zmanTimeJson.get<std::string>();

    //             parseJsonData(zmanimData, zmanDateStr, zmanTimeStr, zmanIdStr);
    //         }
    //     }
    // }
    // else {
    //     for (auto& [zmanIdStr, zmanTimeJson] : timesJson.items()) {
    //         std::string zmanTimeStr = zmanTimeJson.get<std::string>();
    //         std::string zmanDateStr = dateJson.get<std::string>();

    //         parseJsonData(zmanimData, zmanDateStr, zmanTimeStr, zmanIdStr);
    //     }
    // }

    // // for (auto [zmanimDate, b] : zmanimData) {
    // //     std::cout << '\n' << zmanimDate.tm_mon + 1 << '/' << zmanimDate.tm_mday << '/' << zmanimDate.tm_year + 1900
    // //         << ":\n";
    // //     for (auto [zmanId, zmanTime] : b) {
    // //         std::cout << zmanId << ' ' << ((zmanTime.tm_hour % 12) ? (zmanTime.tm_hour % 12) : 12) << ':' << zmanTime.tm_min << ' '
    // //             << (zmanTime.tm_hour / 12 ? "PM" : "AM") << '\n';
    // //     }
    // // }


    // // Build document info

    // std::vector<DayInfo> days;

    // for (auto& [zmanimDate, zmanimMap] : zmanimData) {
    //     std::vector<ZmanInfo> zmanim;

    //     for (auto& [zmanId, zmanTm] : zmanimMap) {

    //     }
    // }

    // return DocumentInfo(std::vector<DayInfo>(), "", "", "");
}