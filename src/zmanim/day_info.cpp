#include <vector>
#include <sstream>
#include <regex>

#include "day_info.h"
#include "time_util.h"
#include "zman_info.h"

std::string DayInfo::htmlTemplate = ""
#include "../html/day.html"
;

DayInfo::DayInfo(
    tm newDate,
    std::string newHoliday,
    std::string newParsha,
    std::vector<ZmanInfo> newZmanim
) : date(newDate), holiday(newHoliday), parsha(newParsha), zmanim(newZmanim) {
    std::stringstream zmanimHtml;

    for (auto zman : newZmanim) {
        zmanimHtml << zman.getHtml();
    }

    std::stringstream dateString;
    dateString << DateToString::monthToString(newDate.tm_mon)
        << ' ' << newDate.tm_mday << DateToString::dateToSuffix(newDate.tm_mday) << ", " << newDate.tm_year + 1900;

    html = htmlTemplate;

    html = std::regex_replace(html, std::regex(R"(\{date\})"), dateString.str());
    html = std::regex_replace(html, std::regex(R"(\{hebrew_date\})"), "");
    html = std::regex_replace(html, std::regex(R"(\{holiday\})"), newHoliday);
    html = std::regex_replace(html, std::regex(R"(\{parsha\})"), newParsha);
    html = std::regex_replace(html, std::regex(R"(\{zmanim\})"), zmanimHtml.str());
};

tm DayInfo::getDate() const { return date; }

std::string DayInfo::getHoliday() const { return holiday; }

std::string DayInfo::getParsha() const { return parsha; }

std::vector<ZmanInfo> DayInfo::getZmanim() const { return zmanim; }

std::string DayInfo::getHtml() const { return html; }