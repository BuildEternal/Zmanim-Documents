#pragma once

#include <ctime>
#include <string>

#include "../web/forecast.h"
#include "../web/zmanim.h"

class DayHtml {
    std::string html;
public:
    DayHtml(
        const tm& date,
        const std::string& hebrewDate,
        const DayZmanimData& zmanimData,
        const ForecastEntry& dayForecast,
        const ForecastEntry& nightForecast
    );

    const std::string& getHtml() const;
};