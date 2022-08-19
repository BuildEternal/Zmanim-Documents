#pragma once

#include <ctime>
#include <map>
#include <string>
#include <vector>

#include "../time_util.h"

class Zman {
    std::string title;
    std::string zman;
    std::string type;
public:
    Zman(const std::string& title, const std::string& zman, const std::string& type);

    const std::string& getTitle() const;
    const std::string& getZman() const;
    const std::string& getType() const;
};

class DayZmanimData {
    std::string holiday;
    std::string parsha;
    std::vector<Zman> zmanim;
public:
    DayZmanimData(const std::string& holiday, const std::string& parsha, const std::vector<Zman>& zmanim);

    const std::string& getHoliday() const;
    const std::string& getParsha() const;
    const std::vector<Zman>& getZmanim() const;
};

typedef std::map<tm, DayZmanimData, decltype(&lessTmDateOnly)> DayZmanimDataMap;

class ZmanimData {
    std::string locationName;
    DayZmanimDataMap dayData;
public:
    ZmanimData(const std::string& locationName, const DayZmanimDataMap& dayData);

    const std::string& getLocationName() const;
    const DayZmanimDataMap& getDayData() const;
};

ZmanimData getZmanim(const DateRange& dates, const std::string& zipCode);