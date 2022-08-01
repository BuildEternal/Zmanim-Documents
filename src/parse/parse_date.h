#pragma once

#include <ctime>
#include <vector>
#include <string>

#include "timespan.h"

Timespan parseTimespan(std::vector<std::string>& args);

tm parseDate(std::vector<std::string>& args);