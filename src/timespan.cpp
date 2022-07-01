#include <ctime>

#include <string>
#include <vector>

#include "timespan.h"

Timespan::Timespan(tm start, tm end) {
    this->start = start;
    this->end = end;
}

Timespan::Timespan(tm time) {
    this->start = time;
    this->end = time;
}

tm Timespan::getStart() const {
    return this->start;
}

tm Timespan::getEnd() const {
    return this->end;
}