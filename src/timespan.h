#pragma once

#include <ctime>

class Timespan {
    tm start;
    tm end;
public:
    Timespan(tm start, tm end);
    Timespan(tm time);

    tm getStart() const;
    tm getEnd() const;
};