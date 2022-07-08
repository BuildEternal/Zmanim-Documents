#pragma once

#include <vector>

#include "day_info.h"

class DocumentInfo {
    static std::string htmlTemplate;

    std::vector<DayInfo> days;
    std::string location;
    std::string locationDetails;
    std::string locationId;
    std::string html;
public:
    DocumentInfo(
        std::vector<DayInfo> newDays,
        std::string newLocation,
        std::string newLocationDetails,
        std::string newLocationId
    );

    std::vector<DayInfo> getDays() const;
    std::string getLocation() const;
    std::string getLocationDetails() const;
    std::string getLocationId() const;
    std::string getHtml() const;
};