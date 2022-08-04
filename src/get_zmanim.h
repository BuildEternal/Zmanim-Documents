#pragma once

#include <string>

#include "timespan.h"
#include "html_builders/document_info.h"

DocumentInfo get_zmanim(
    const Timespan& timespan, std::string locationType, std::string locationId,
    double forecastLat, double forecastLong
);