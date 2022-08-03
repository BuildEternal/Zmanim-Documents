#include <regex>
#include <string>

#include "weather_entry_info.h"

std::string WeatherEntryInfo::htmlTemplate = ""
#include "html/weather_entry.txt"
;

WeatherEntryInfo::WeatherEntryInfo(
    bool newDay, int newTemp, std::string newTempUnit, std::string newWindDirection,
    std::string newWindSpeed, std::string newDetails
) : day(newDay), temp(newTemp), tempUnit(newTempUnit),
windDirection(newWindDirection), windSpeed(newWindSpeed), html(htmlTemplate), details(newDetails)
{
    std::string windDirStr =
        newWindDirection == "N" ? "⬆️" :
        newWindDirection == "NE" ? "↗️" :
        newWindDirection == "E" ? "➡️" :
        newWindDirection == "SE" ? "↘️" :
        newWindDirection == "S" ? "⬇️" :
        newWindDirection == "SW" ? "↙️" :
        newWindDirection == "W" ? "⬅️" :
        newWindDirection == "NW" ? "↖️" : "❓";

    html = std::regex_replace(
        html, std::regex(R"(\{if_day\s*([\s\S]*?)\s*if_day\})"), day ? "$1" : ""
    );
    html = std::regex_replace(
        html, std::regex(R"(\{if_night\s*([\s\S]*?)\s*if_night\})"), day ? "" : "$1"
    );

    html = std::regex_replace(html, std::regex(R"(\{temperature\})"), std::to_string(temp) + "&deg;" + tempUnit);
    html = std::regex_replace(html, std::regex(R"(\{wind\})"), windDirStr + ' ' + newWindSpeed);
    html = std::regex_replace(html, std::regex(R"(\{details\})"), newDetails);
}

WeatherEntryInfo::WeatherEntryInfo() { }

int WeatherEntryInfo::getTemp() const { return temp; }

std::string WeatherEntryInfo::getTempUnit() const { return tempUnit; }

std::string WeatherEntryInfo::getWindDirection() const { return windDirection; }

std::string WeatherEntryInfo::getWindSpeed() const { return windSpeed; }

std::string WeatherEntryInfo::getDetails() const { return details; }

std::string WeatherEntryInfo::getHtml() const { return html; }