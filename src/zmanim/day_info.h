#pragma once

#include <iostream>
#include <vector>

#include "forecast.h"
#include "zman_info.h"

class DayInfo {
    static std::string htmlTemplate;

    tm date;
    std::string holiday;
    std::string parsha;
    std::vector<ZmanInfo> zmanim;
    std::string html;
    WeatherEntryInfo dayForecast;
    WeatherEntryInfo nightForecast;
public:
    DayInfo(
        tm newDate,
        std::string newHoliday,
        std::string newParsha,
        std::vector<ZmanInfo> newZmanim,
        WeatherEntryInfo newDayForecast,
        WeatherEntryInfo newNightForecast
    );

    tm getDate() const;
    std::string getHoliday() const;
    std::string getParsha() const;
    std::vector<ZmanInfo> getZmanim() const;
    WeatherEntryInfo getDayForecast() const;
    WeatherEntryInfo getNightForecast() const;
    std::string getHtml() const;
};