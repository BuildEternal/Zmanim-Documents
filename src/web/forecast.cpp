#include <ctime>
#include <iostream>
#include <map>
#include <string>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>
#include <nlohmann/json.hpp>

#include "time_util.h"

#include "forecast.h"

using nlohmann::json;

// ForecastInfo

ForecastEntry::ForecastEntry(
    int temp,
    const std::string& tempUnit,
    const std::string& windDirection,
    const std::string& windSpeed,
    const std::string& details
) :
    temp(temp),
    tempUnit(tempUnit),
    windDirection(windDirection),
    windSpeed(windSpeed),
    details(details) { };
ForecastEntry::ForecastEntry() : empty(true) { };

bool ForecastEntry::isEmpty() const { return empty; }

int ForecastEntry::getTemp() const { return temp; };
const std::string& ForecastEntry::getTempUnit() const { return tempUnit; };
const std::string& ForecastEntry::getWindDirection() const { return windDirection; };
const std::string& ForecastEntry::getWindSpeed() const { return windSpeed; };
const std::string& ForecastEntry::getDetails() const { return details; };

// Forecast

Forecast::Forecast(const ForecastInfoMap& forecastInfo) :
    forecastInfo(forecastInfo) { };
Forecast::Forecast() : empty(true) { };

bool Forecast::isEmpty() const { return empty; }
const ForecastInfoMap& Forecast::getForecastInfo() const { return forecastInfo; }

// getForecast

Forecast getForecast(double latitude, double longitude) {
    httplib::Client weatherCli("https://api.weather.gov");
    weatherCli.set_follow_location(true);

    auto gridInfoRes = weatherCli.Get("/points/" + std::to_string(latitude) + ',' + std::to_string(longitude));

    if (gridInfoRes->status != 200) throw std::runtime_error("There was an error getting grid info from coordinates.");

    json gridInfo = json::parse(gridInfoRes->body)["properties"];

    std::string gridId = gridInfo["gridId"].get<std::string>();
    int gridX = gridInfo["gridX"].get<int>();
    int gridY = gridInfo["gridY"].get<int>();

    auto forecastRes = weatherCli.Get(
        "/gridpoints/" + gridId + '/' + std::to_string(gridX) + ',' + std::to_string(gridY) + "/forecast");

    if (forecastRes->status != 200) throw std::runtime_error("There was an error getting the forecast.");

    json forecastJson = json::parse(forecastRes->body)["properties"];

    ForecastInfoMap forecastInfoMap(&lessTmDateOnly);

    json& forecastInfoJsonArray = forecastJson["periods"];

    for (auto& forecastInfoJson : forecastInfoJsonArray) {
        tm date = stringToTm(forecastInfoJson["startTime"].get<std::string>());
        bool dayTime = forecastInfoJson["isDaytime"].get<bool>();

        int temp = forecastInfoJson["temperature"].get<int>();
        std::string tempUnit = forecastInfoJson["temperatureUnit"].get<std::string>();
        std::string windDirection = forecastInfoJson["windDirection"].get<std::string>();
        std::string windSpeed = forecastInfoJson["windSpeed"].get<std::string>();
        std::string details = forecastInfoJson["detailedForecast"].get<std::string>();

        forecastInfoMap[date].emplace(dayTime, ForecastEntry(temp, tempUnit, windDirection, windSpeed, details));
    }

    return Forecast(forecastInfoMap);
}