#include <regex>

#include "../web/forecast.h"
#include "../web/zmanim.h"
#include "forecast_entry_html.h"
#include "time_util.h"
#include "zman_html.h"

#include "day_html.h"

const std::string htmlTemplate = ""
#include "../html/day.txt"
;

DayHtml::DayHtml(
    const tm& date,
    const std::string& hebrewDate,
    const DayZmanimData& zmanimData,
    const ForecastEntry& dayForecast,
    const ForecastEntry& nightForecast
) {
    // Zmanim HTML
    std::string zmanimHtml;

    for (const auto& zman : zmanimData.getZmanim()) zmanimHtml += ZmanHtml(zman).getHtml();

    // Date
    std::string dateString = tmToPrettyDateString(date, true);

    html = std::regex_replace(
        htmlTemplate, std::regex(R"(\{if_holiday\s*([\s\S]*?)\s*if_holiday\})"),
        zmanimData.getHoliday() != "" ? "$1" : "");
    html = std::regex_replace(
        html, std::regex(R"(\{if_parsha\s*([\s\S]*?)\s*if_parsha\})"),
        zmanimData.getParsha() != "" ? "$1" : "");
    html = std::regex_replace(
        html, std::regex(R"(\{if_hebrew_date\s*([\s\S]*?)\s*if_hebrew_date\})"),
        hebrewDate != "" ? "$1" : "");

    html = std::regex_replace(html, std::regex(R"(\{date\})"), dateString);
    html = std::regex_replace(html, std::regex(R"(\{hebrew_date\})"), hebrewDate);
    html = std::regex_replace(html, std::regex(R"(\{holiday\})"), zmanimData.getHoliday());
    html = std::regex_replace(html, std::regex(R"(\{parsha\})"), zmanimData.getParsha());
    html = std::regex_replace(html, std::regex(R"(\{zmanim\})"), zmanimHtml);
    html = std::regex_replace(html, std::regex(R"(\{day_weather\})"),
        ForecastEntryHtml(true, dayForecast).getHtml());
    html = std::regex_replace(html, std::regex(R"(\{night_weather\})"),
        ForecastEntryHtml(false, nightForecast).getHtml());
};

const std::string& DayHtml::getHtml() const { return html; }