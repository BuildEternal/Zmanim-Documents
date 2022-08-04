#include <regex>
#include <vector>
#include <sstream>

#include "document_info.h"
#include "day_info.h"

std::string DocumentInfo::htmlTemplate = ""
#include "../html/document.txt"
;

DocumentInfo::DocumentInfo(
    std::vector<DayInfo> newDays,
    std::string newLocation,
    std::string newLocationDetails,
    std::string newLocationId
) : days(newDays), location(newLocation), locationDetails(newLocationDetails), locationId(newLocationId) {
    std::stringstream daysHtml;

    // std::cout << "size: " << newDays.size() << std::endl;

    for (auto dayInfo : newDays) {
        // std::cout << "loop" << std::endl;
        // std::cout << days.size() << ' ' << dayInfo.getDate().tm_year + 1900 << std::endl;
        // std::cout << dayInfo.getHtml();
        daysHtml << dayInfo.getHtml();
    }

    html = htmlTemplate;

    html = std::regex_replace(html, std::regex(R"(\{days\})"), daysHtml.str());
}

std::vector<DayInfo> DocumentInfo::getDays() const { return days; }

std::string DocumentInfo::getLocation() const { return location; }

std::string DocumentInfo::getLocationDetails() const { return locationDetails; }

std::string DocumentInfo::getLocationId() const { return locationId; }

std::string DocumentInfo::getHtml() const { return html; }