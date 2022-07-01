#include <vector>

#include "day_info.h"
#include "zman_info.h"

DayInfo::DayInfo(
    tm date,
    std::string holiday,
    std::string parsha,
    std::vector<ZmanInfo> zmanim
) : date(date), holiday(holiday), parsha(parsha), zmanim(zmanim) {};

tm DayInfo::getDate() const { return date; }

std::string DayInfo::getHoliday() const { return holiday; }

std::string DayInfo::getParsha() const { return parsha; }

std::vector<ZmanInfo> DayInfo::getZmanim() const { return zmanim; }