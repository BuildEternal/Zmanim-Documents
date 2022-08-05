#pragma once

#include <ctime>
#include <map>
#include <string>

#include "time_util.h"

class ForecastEntry {
    bool empty = false;

    int temp;
    std::string tempUnit;
    std::string windDirection;
    std::string windSpeed;
    std::string details;
public:
    ForecastEntry(
        int temp,
        const std::string& tempUnit,
        const std::string& windDirection,
        const std::string& windSpeed,
        const std::string& details
    );
    ForecastEntry();

    bool isEmpty() const;

    int getTemp() const;
    const std::string& getTempUnit() const;
    const std::string& getWindDirection() const;
    const std::string& getWindSpeed() const;
    const std::string& getDetails() const;
};

typedef std::map<tm, std::map<bool, ForecastEntry>, decltype(&lessTmDateOnly)> ForecastInfoMap;

class Forecast {
    bool empty = false;

    ForecastInfoMap forecastInfo;
public:
    Forecast(const ForecastInfoMap& forecastInfo);
    Forecast();

    bool isEmpty() const;
    const ForecastInfoMap& getForecastInfo() const;
};

Forecast getForecast(double latitude, double longitude);