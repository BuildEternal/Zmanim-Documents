#pragma once

#include <string>

#include "../web/forecast.h"
#include "../web/hebrew_dates.h"
#include "../web/zmanim.h"
#include "day_html.h"

class DocumentHtml {
    std::string html;
public:
    DocumentHtml(const ZmanimData& zmanim, const HebrewDateMap& hebrewDates, const Forecast& forecast);

    const std::string& getHtml() const;
};