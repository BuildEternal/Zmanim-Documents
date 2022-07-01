#pragma once

#include <iostream>
#include <vector>

#include "zman_info.h"

class DayInfo {
    tm date;
    std::string holiday;
    std::string parsha;
    std::vector<ZmanInfo> zmanim;
public:
    DayInfo(tm date, std::string holiday, std::string parsha, std::vector<ZmanInfo> zmanim);

    tm getDate() const;
    std::string getHoliday() const;
    std::string getParsha() const;
    std::vector<ZmanInfo> getZmanim() const;
};