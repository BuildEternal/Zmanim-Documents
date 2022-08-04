#include <vector>
#include <sstream>
#include <regex>

#include "day_info.h"
#include "forecast.h"
#include "time_util.h"
#include "zman_info.h"

std::string DayInfo::htmlTemplate = ""
#include "../html/day.txt"
;

DayInfo::DayInfo(
    tm newDate,
    std::string newHeDate,
    std::string newHoliday,
    std::string newParsha,
    std::vector<ZmanInfo> newZmanim,
    WeatherEntryInfo newDayForecast,
    WeatherEntryInfo newNightForecast
) : date(newDate), holiday(newHoliday), parsha(newParsha), zmanim(newZmanim), dayForecast(newDayForecast),
nightForecast(newNightForecast)
{
    // Zmanim HTML
    std::stringstream zmanimHtml;

    for (auto zman : newZmanim) {
        zmanimHtml << zman.getHtml();
    }

    // Date
    std::stringstream dateString;
    dateString << dayToString(newDate.tm_wday) << ", " << monthToString(newDate.tm_mon)
        << ' ' << newDate.tm_mday << ", " << newDate.tm_year + 1900;

    html = htmlTemplate;

    html = std::regex_replace(
        html, std::regex(R"(\{if_holiday\s*([\s\S]*?)\s*if_holiday\})"), (newHoliday != "") ? "$1" : ""
    );
    html = std::regex_replace(
        html, std::regex(R"(\{if_parsha\s*([\s\S]*?)\s*if_parsha\})"), (newParsha != "") ? "$1" : ""
    );
    html = std::regex_replace(
        html, std::regex(R"(\{if_hebrew_date\s*([\s\S]*?)\s*if_hebrew_date\})"), (newHeDate != "") ? "$1" : ""
    );

    html = std::regex_replace(html, std::regex(R"(\{date\})"), dateString.str());
    html = std::regex_replace(html, std::regex(R"(\{hebrew_date\})"), newHeDate);
    html = std::regex_replace(html, std::regex(R"(\{holiday\})"), newHoliday);
    html = std::regex_replace(html, std::regex(R"(\{parsha\})"), newParsha);
    html = std::regex_replace(html, std::regex(R"(\{zmanim\})"), zmanimHtml.str());
    html = std::regex_replace(html, std::regex(R"(\{day_weather\})"), newDayForecast.getHtml());
    html = std::regex_replace(html, std::regex(R"(\{night_weather\})"), newNightForecast.getHtml());
};

tm DayInfo::getDate() const { return date; }

std::string DayInfo::getHeDate() const { return heDate; }

std::string DayInfo::getHoliday() const { return holiday; }

std::string DayInfo::getParsha() const { return parsha; }

std::vector<ZmanInfo> DayInfo::getZmanim() const { return zmanim; }

WeatherEntryInfo DayInfo::getDayForecast() const { return dayForecast; }

WeatherEntryInfo DayInfo::getNightForecast() const { return nightForecast; }

std::string DayInfo::getHtml() const { return html; }