#pragma once

#include <ctime>
#include <map>
#include <string>

#include "../time_util.h"

typedef std::map<tm, std::string, decltype(&lessTmDateOnly)> HebrewDateMap;

HebrewDateMap getHebrewDates(const DateRange& dates);