#pragma once

#include <vector>

#include "day_info.h"

class DocumentInfo {
    std::vector<DayInfo> days;
    std::string location;
    std::string locationDetails;
    std::string locationId;
public:
    DocumentInfo(std::vector<DayInfo> days, std::string location, std::string locationDetails, std::string locationId);

    std::vector<DayInfo> getDays() const;
    std::string getLocation() const;
    std::string getLocationDetails() const;
    std::string getLocationId() const;
};