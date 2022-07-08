#pragma once

#include <iostream>
#include <vector>

#include "zman_info.h"

class DayInfo {
    static std::string htmlTemplate;

    tm date;
    std::string holiday;
    std::string parsha;
    std::vector<ZmanInfo> zmanim;
    std::string html;
public:
    DayInfo(tm newDate, std::string newHoliday, std::string newParsha, std::vector<ZmanInfo> newZmanim);

    tm getDate() const;
    std::string getHoliday() const;
    std::string getParsha() const;
    std::vector<ZmanInfo> getZmanim() const;
    std::string getHtml() const;
};