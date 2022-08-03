#pragma once

#include <map>
#include <string>

#include "time_util.h"
#include "timespan.h"

// Gets Hebrew dates for the given timespan.
std::map<tm, std::string, decltype(&lessTmDateOnly)> getHebrewDates(Timespan dates);