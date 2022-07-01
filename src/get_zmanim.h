#pragma once

#include <nlohmann/json.hpp>

#include "timespan.h"
#include "zmanim/document_info.h"

DocumentInfo get_zmanim(const Timespan& timespan, int locationType, int locationId);
