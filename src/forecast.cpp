#include <iostream>
#include <string>
#include <map>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>
#include <nlohmann/json.hpp>

#include "forecast.h"
#include "time_util.h"
#include "zmanim/weather_entry_info.h"

using nlohmann::json;

// Forecast::Forecast(
//     std::map<tm, std::map<bool, ForecastInfo>, decltype(lessTmDateOnly)> newForecastInfo, tm newGeneratedAt
// ) : forecastInfo(newForecastInfo), generatedAt(newGeneratedAt) { };

// const std::map<tm, std::map<bool, ForecastInfo>, decltype(lessTmDateOnly)>& Forecast::getForecastInfo() const {
//     return forecastInfo;
// }

// tm Forecast::getGeneratedAt() const { return generatedAt; }

std::map<tm, std::map<bool, WeatherEntryInfo>, decltype(&lessTmDateOnly)> getForecastForCoords(
    double latitude, double longitude
) {
    httplib::Client weatherCli("https://api.weather.gov");
    weatherCli.set_follow_location(true);

    auto gridInfoRes = weatherCli.Get("/points/" + std::to_string(latitude) + ',' + std::to_string(longitude));

    if (gridInfoRes->status != 200) throw std::runtime_error("Error getting grid info from coordinates.");

    json gridInfo = json::parse(gridInfoRes->body)["properties"];

    std::string gridId = gridInfo["gridId"].get<std::string>();
    int gridX = gridInfo["gridX"].get<int>();
    int gridY = gridInfo["gridY"].get<int>();


    auto forecastRes = weatherCli.Get(
        "/gridpoints/" + gridId + '/' + std::to_string(gridX) + ',' + std::to_string(gridY) + "/forecast");

    std::cout << forecastRes->status << '\n';

    if (forecastRes->status != 200) throw std::runtime_error("Error getting the forecast.");

    json forecastJson = json::parse(forecastRes->body)["properties"];

    std::map<tm, std::map<bool, WeatherEntryInfo>, decltype(&lessTmDateOnly)> forecastInfoMap(&lessTmDateOnly);

    json forecastInfoJsonArray = forecastJson["periods"];

    for (auto forecastInfoJson : forecastInfoJsonArray) {
        tm date = tmFromString(forecastInfoJson["startTime"].get<std::string>());
        bool dayTime = forecastInfoJson["isDaytime"].get<bool>();

        int temp = forecastInfoJson["temperature"].get<int>();
        std::string tempUnit = forecastInfoJson["temperatureUnit"].get<std::string>();
        std::string windDirection = forecastInfoJson["windDirection"].get<std::string>();
        std::string windSpeed = forecastInfoJson["windSpeed"].get<std::string>();
        std::string details = forecastInfoJson["detailedForecast"].get<std::string>();

        forecastInfoMap[date].emplace(dayTime, WeatherEntryInfo(
            dayTime, temp, tempUnit, windDirection, windSpeed, details
        ));
    }

    return forecastInfoMap;
}