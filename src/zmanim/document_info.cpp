#include <vector>

#include "document_info.h"
#include "day_info.h"

DocumentInfo::DocumentInfo(
    std::vector<DayInfo> days,
    std::string location,
    std::string locationDetails,
    std::string locationId
) : days(days), location(location), locationDetails(locationDetails), locationId(locationId) {}

std::vector<DayInfo> DocumentInfo::getDays() const { return days; }

std::string DocumentInfo::getLocation() const { return location; }

std::string DocumentInfo::getLocationDetails() const { return locationDetails; }

std::string DocumentInfo::getLocationId() const { return locationId; }