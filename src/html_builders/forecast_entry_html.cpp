#include <regex>
#include <string>

#include "../web/forecast.h"

#include "forecast_entry_html.h"

const std::string htmlTemplate = ""
#include "html/weather_entry.txt"
;

ForecastEntryHtml::ForecastEntryHtml(bool isDay, const ForecastEntry& forecastEntry) {
    if (forecastEntry.isEmpty()) return;

    const std::string& windDirection = forecastEntry.getWindDirection();

    std::string windDirStr =
        windDirection == "N" ? "⬆️" :
        windDirection == "NE" ? "↗️" :
        windDirection == "E" ? "➡️" :
        windDirection == "SE" ? "↘️" :
        windDirection == "S" ? "⬇️" :
        windDirection == "SW" ? "↙️" :
        windDirection == "W" ? "⬅️" :
        windDirection == "NW" ? "↖️" : "❓";

    html = std::regex_replace(
        htmlTemplate, std::regex(R"(\{if_day\s*([\s\S]*?)\s*if_day\})"), isDay ? "$1" : ""
    );
    html = std::regex_replace(
        html, std::regex(R"(\{if_night\s*([\s\S]*?)\s*if_night\})"), isDay ? "" : "$1"
    );

    html = std::regex_replace(html, std::regex(R"(\{temperature\})"),
        std::to_string(forecastEntry.getTemp()) + "&deg;" + forecastEntry.getTempUnit());
    html = std::regex_replace(html, std::regex(R"(\{wind\})"), windDirStr + ' ' + forecastEntry.getWindSpeed());
    html = std::regex_replace(html, std::regex(R"(\{details\})"), forecastEntry.getDetails());
}

const std::string& ForecastEntryHtml::getHtml() const { return html; }