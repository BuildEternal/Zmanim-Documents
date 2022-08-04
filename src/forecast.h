#pragma once

#include <ctime>
#include <string>
#include <map>

#include "time_util.h"
#include "html_builders/weather_entry_info.h"

// class Forecast {
//     std::map<tm, std::map<bool, ForecastInfo>, decltype(lessTmDateOnly)> forecastInfo;
//     tm generatedAt;
// public:
//     Forecast(std::map<tm, std::map<bool, ForecastInfo>, decltype(lessTmDateOnly)> newForecastInfo, tm newGeneratedAt);

//     const std::map<tm, std::map<bool, ForecastInfo>, decltype(lessTmDateOnly)>& getForecastInfo() const;
//     tm getGeneratedAt() const;
// };



std::map<tm, std::map<bool, WeatherEntryInfo>, decltype(&lessTmDateOnly)> getForecastForCoords(
    double latitude, double longitude
);