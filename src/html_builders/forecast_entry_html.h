#pragma once

#include <string>

#include "../web/forecast.h"

class ForecastEntryHtml {
    std::string html;
public:
    ForecastEntryHtml(bool isDay, const ForecastEntry& forecastEntry);

    const std::string& getHtml() const;
};