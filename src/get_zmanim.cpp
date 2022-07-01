#include <iostream>
#include <ctime>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "cpp-httplib/httplib.h"
#include "timespan.h"
#include "json/include/nlohmann/json.hpp"
#include "zmanim/day_info.h"
#include "zmanim/document_info.h"
#include "zmanim/zman_info.h"
#include "time_util.h"

using json = nlohmann::json;

/**
 * @brief Gets the zmanim for the given timespan.
 *
 * @param timespan The zmanim range
 * @return The zmanim JSON data
 */
DocumentInfo get_zmanim(const Timespan& timespan, int locationType, int locationId) {
    httplib::Client cli("https://www.chabad.org");

    cli.set_default_headers({
            { "Accept", "application/json" }
        });

    tm start = timespan.getStart();
    tm end = timespan.getEnd();

    std::stringstream startParamStream;
    startParamStream << "startdate=" << start.tm_mon + 1 << "%2F" << start.tm_mday << "%2F" <<
        start.tm_year + 1900;

    std::stringstream endParamStream;
    endParamStream << "enddate=" << end.tm_mon + 1 << "%2F" << end.tm_mday << "%2F" <<
        end.tm_year + 1900;

    std::stringstream locationTypeParamStream;
    locationTypeParamStream << "locationtype=" << locationType;

    std::stringstream locationIdParamStream;
    locationIdParamStream << "locationid=" << locationId;

    std::stringstream paramsStream;
    paramsStream << '?' << startParamStream.str() << '&' << endParamStream.str() << '&' << locationTypeParamStream.str()
        << '&' << locationIdParamStream.str();

    std::stringstream requestStream;
    requestStream << "/webservices/zmanim/zmanim/Get_Zmanim" << paramsStream.str();

    auto res = cli.Get(requestStream.str().c_str());

    if (!(res->status == 200)) throw std::runtime_error("An error accured when getting zmanim.");

    auto zmanimJson = json::parse(res->body);

    // Extract data from JSON

    std::vector<DayInfo> days;

    json daysJsonArray = zmanimJson["Days"];

    for (auto i = daysJsonArray.begin(); i < daysJsonArray.end(); i++) {
        json dayJson = *i;
        auto daysIndex = i - daysJsonArray.begin();

        tm date = start;
        date.tm_mday += daysIndex;
        date = normalizeTm(date);

        std::cout << "Date: " << date.tm_mon << '/' << date.tm_mday << '/' << date.tm_year << std::endl;

        std::string holiday = ""; {
            json holidayJson = dayJson["HolidayName"];

            if (holidayJson.type() == json::value_t::string)
                holiday = holidayJson.get<std::string>();
        }

        std::cout << "Holiday: " << holiday << std::endl;

        std::string parsha = ""; {
            json parshaJson = dayJson["Parsha"];

            if (parshaJson.type() == json::value_t::string)
                parsha = parshaJson.get<std::string>();
        }

        std::cout << "Parsha: " << parsha << std::endl;

        std::vector<ZmanInfo> zmanim;

        json zmanimJsonArray = dayJson["TimeGroups"];

        for (auto j = zmanimJsonArray.begin(); j < zmanimJsonArray.end(); j++) {
            json zmanJson = *j;

            std::string title = zmanJson["Title"].get<std::string>();
            std::string zman = zmanJson["Items"][0]["Zman"].get<std::string>();

            std::cout << "Title: " << title << std::endl;
            std::cout << "Zman: " << zman << std::endl;

            ZmanInfo zmanInfo(title, zman);

            zmanim.push_back(zmanInfo);
        }

        DayInfo dayInfo(date, holiday, parsha, zmanim);

        days.push_back(dayInfo);
    }

    std::string location = zmanimJson["LocationName"].get<std::string>();

    std::cout << "Location: " << location << std::endl;

    std::string locationDetails = zmanimJson["LocationDetails"].get<std::string>();

    std::cout << "Location Details: " << locationDetails << std::endl;

    std::string returnedLocationId = zmanimJson["LocationId"].get<std::string>();

    std::cout << "Location ID: " << returnedLocationId << std::endl;

    DocumentInfo documentInfo(days, location, locationDetails, returnedLocationId);

    return documentInfo;
}