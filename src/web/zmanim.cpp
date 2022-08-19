#include <ctime>
#include <map>
#include <string>
#include <vector>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>
#include <nlohmann/json.hpp>

#include "../time_util.h"

#include "zmanim.h"

using nlohmann::json;

Zman::Zman(const std::string& title, const std::string& zman, const std::string& type) :
    title(title), zman(zman), type(type) { }

const std::string& Zman::getTitle() const { return title; }
const std::string& Zman::getZman() const { return zman; }
const std::string& Zman::getType() const { return type; }

DayZmanimData::DayZmanimData(const std::string& holiday, const std::string& parsha, const std::vector<Zman>& zmanim) :
    holiday(holiday), parsha(parsha), zmanim(zmanim) { }

const std::string& DayZmanimData::getHoliday() const { return holiday; }
const std::string& DayZmanimData::getParsha() const { return parsha; }
const std::vector<Zman>& DayZmanimData::getZmanim() const { return zmanim; }

ZmanimData::ZmanimData(
    const std::string& locationName, const DayZmanimDataMap& dayData
) : locationName(locationName), dayData(dayData) { }

const std::string& ZmanimData::getLocationName() const { return locationName; }
const DayZmanimDataMap& ZmanimData::getDayData() const { return dayData; }

httplib::Client chabadCli("https://www.chabad.org");

ZmanimData getZmanim(const DateRange& dates, const std::string& zipCode) {
    const tm& start = dates.getStart();
    const tm& end = dates.getEnd();

    auto res = chabadCli.Get("/webservices/zmanim/zmanim/Get_Zmanim", httplib::Params(
        {
            { "startdate", tmToBasicDateString(start) },
            { "enddate", tmToBasicDateString(end) },
            { "locationtype", "2" },
            { "locationid", zipCode }
        }
    ), httplib::Headers(
        { { "Accept", "application/json" } }
    ));

    if (res->status != 200) throw std::runtime_error("Could not retrieve zmanim.");

    json::object_t resJson = json::parse(res->body);

    json::array_t daysJsonArray = resJson["Days"];

    if (!daysJsonArray[0]["Zmanim"].is_null()) throw std::runtime_error(
        "Cannot organize the zmanim data. The given date range may be too long."
    );

    std::string locationName = resJson["LocationName"].get<std::string>();
    DayZmanimDataMap dayData(&lessTmDateOnly);

    for (auto& dayJson : daysJsonArray) {
        json holidayJson = dayJson["HolidayName"];
        json parshaJson = dayJson["Parsha"];
        json zmanimJsonArray = dayJson["TimeGroups"];

        tm date = stringToTm(dayJson["DisplayDate"].get<std::string>(), true);
        date.tm_wday = dayJson["DayOfWeek"].get<int>();

        std::string holiday = holidayJson.is_string() ? holidayJson.get<std::string>() : "";
        std::string parsha = parshaJson.is_string() ? parshaJson.get<std::string>() : "";
        std::vector<Zman> zmanim;

        for (auto& zmanJson : zmanimJsonArray) {
            std::string title = zmanJson["Title"].get<std::string>();
            std::string zman = zmanJson["Items"][0]["Zman"].get<std::string>();
            std::string type = zmanJson["Items"][0]["ZmanType"].get<std::string>();

            zmanim.push_back(Zman(title, zman, type));
        }

        dayData.emplace(date, DayZmanimData(holiday, parsha, zmanim));
    }

    return ZmanimData(locationName, dayData);
}