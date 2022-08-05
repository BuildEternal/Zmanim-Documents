#pragma once

#include <ctime>
#include <string>

// Date validation

tm normalizeTm(tm toNormalize);
bool validTmDateOnly(const tm& check);

// Date number to text

std::string monthToString(int month);
std::string dayToString(int wday);

// tm-string conversion

tm stringToTm(std::string str, bool basicDate = false);
std::string tmToBasicDateString(tm tmToConvert);
std::string tmToIsoDateString(tm tmToConvert);
std::string tmToPrettyDateString(tm tmToConvert, bool includeWeekday = false, bool includeTime = false);

// tm comparison

bool lessTmDateOnly(const tm& a, const tm& b);

// Date range

class DateRange {
    tm start;
    tm end;
public:
    DateRange(const tm& start, const tm& end);
    DateRange(const tm& time);

    const tm& getStart() const;
    const tm& getEnd() const;
};