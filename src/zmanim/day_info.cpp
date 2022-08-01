#include <vector>
#include <sstream>
#include <regex>

#include "day_info.h"
#include "time_util.h"
#include "zman_info.h"

std::string DayInfo::htmlTemplate = ""
#include "../html/day.txt"
;

DayInfo::DayInfo(
    tm newDate,
    std::string newHoliday,
    std::string newParsha,
    std::vector<ZmanInfo> newZmanim
) : date(newDate), holiday(newHoliday), parsha(newParsha), zmanim(newZmanim) {
    // Zmanim HTML
    std::stringstream zmanimHtml;

    for (auto zman : newZmanim) {
        zmanimHtml << zman.getHtml();
    }

    // Date
    std::stringstream dateString;
    dateString << monthToString(newDate.tm_mon)
        << ' ' << newDate.tm_mday << dateToSuffix(newDate.tm_mday) << ", " << newDate.tm_year + 1900;

    html = htmlTemplate;

    html = std::regex_replace(
        html, std::regex(R"(\{if_holiday\s*([\s\S]*?)\s*if_holiday\})"), (newHoliday != "") ? "$1" : ""
    );

    html = std::regex_replace(
        html, std::regex(R"(\{if_parsha\s*([\s\S]*?)\s*if_parsha\})"), (newParsha != "") ? "$1" : ""
    );

    html = std::regex_replace(html, std::regex(R"(\{date\})"), dateString.str());
    html = std::regex_replace(html, std::regex(R"(\{hebrew_date\})"), "TODO");
    html = std::regex_replace(html, std::regex(R"(\{holiday\})"), newHoliday);
    html = std::regex_replace(html, std::regex(R"(\{parsha\})"), newParsha);
    html = std::regex_replace(html, std::regex(R"(\{zmanim\})"), zmanimHtml.str());
};

tm DayInfo::getDate() const { return date; }

std::string DayInfo::getHoliday() const { return holiday; }

std::string DayInfo::getParsha() const { return parsha; }

std::vector<ZmanInfo> DayInfo::getZmanim() const { return zmanim; }

std::string DayInfo::getHtml() const { return html; }