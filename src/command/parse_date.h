#pragma once

#include <ctime>
#include <vector>
#include <string>

#include "../time_util.h"

DateRange parseTimespan(std::vector<std::string>& args);

tm parseDate(std::vector<std::string>& args);