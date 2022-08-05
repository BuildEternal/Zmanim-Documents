#include <ctime>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <stdexcept>

#include "time_util.h"

tm normalizeTm(tm toNormalize) {
    time_t asTimeT = mktime(&toNormalize);

    if (asTimeT == -1)
        throw std::invalid_argument("A time was invalid.");

    return *localtime(&asTimeT);
}

bool validTmDateOnly(const tm& check) {
    auto copy = check;

    try {
        copy = normalizeTm(copy);
    }
    catch (std::invalid_argument) {
        return false;
    }

    return check.tm_mday == copy.tm_mday &&
        check.tm_mon == copy.tm_mon &&
        check.tm_year == copy.tm_year;
}

std::string monthToString(int month) {
    switch (month) {
    case 0: return "January";
    case 1: return "February";
    case 2: return "March";
    case 3: return "April";
    case 4: return "May";
    case 5: return "June";
    case 6: return "July";
    case 7: return "August";
    case 8: return "September";
    case 9: return "October";
    case 10: return "November";
    case 11: return "December";
    default: return "";
    }
}

std::string dayToString(int wday) {
    switch (wday) {
    case 0: return "Sunday";
    case 1: return "Monday";
    case 2: return "Tuesday";
    case 3: return "Wednesday";
    case 4: return "Thursday";
    case 5: return "Friday";
    case 6: return "Shabbat";
    default: return "";
    }
}

tm stringToTm(std::string str, bool basicDate) {
    tm newTm;

    if (!basicDate) {
        std::regex dateRegex(R"((\d{4})-(\d{2})-(\d{2}))");
        std::smatch dateValues;
        if (std::regex_search(str, dateValues, dateRegex)) {
            newTm.tm_year = stoi(dateValues[1]) - 1900;
            newTm.tm_mon = stoi(dateValues[2]) - 1;
            newTm.tm_mday = stoi(dateValues[3]);
        }

        std::regex timeRegex(R"((\d{2}):(\d{2}):(\d{2}))");
        std::smatch timeValues;
        if (std::regex_search(str, timeValues, timeRegex)) {
            newTm.tm_hour = stoi(timeValues[1]);
            newTm.tm_min = stoi(timeValues[2]);
            newTm.tm_sec = stoi(timeValues[3]);
        }
    }
    else {
        std::regex basicDateRegex(R"(^(\d{1,2})/(\d{1,2})/(\d{1,2}|\d{4})$)");
        std::smatch basicDateVals;
        if (std::regex_search(str, basicDateVals, basicDateRegex)) {
            newTm.tm_mon = stoi(basicDateVals[1]) - 1;
            newTm.tm_mday = stoi(basicDateVals[2]);
            newTm.tm_year = basicDateVals[3].length() <= 2 ?
                stoi(basicDateVals[3]) + 100 : stoi(basicDateVals[3]) - 1900;
        }
    }

    return newTm;
}

// tm to mm/dd/yyyy
std::string tmToBasicDateString(tm tmToConvert) {
    return (std::stringstream() <<
        tmToConvert.tm_mon + 1 << '/' << tmToConvert.tm_mday << '/' << tmToConvert.tm_year + 1900).str();
}

// tm to yyyy-mm-dd
std::string tmToIsoDateString(tm tmToConvert) {
    return (std::stringstream() << std::setfill('0') <<
        std::setw(4) << tmToConvert.tm_year + 1900 << '-' <<
        std::setw(2) << tmToConvert.tm_mon + 1 << '-' <<
        std::setw(2) << tmToConvert.tm_mday).str();
}

// tm to weekday, month day, year
std::string tmToPrettyDateString(tm tmToConvert, bool includeWeekday, bool includeTime) {
    std::string timeStr;

    if (includeTime) {
        int hour = tmToConvert.tm_hour % 12;
        if (!hour) hour = 12;
        int min = tmToConvert.tm_min;
        std::string meridiem = tmToConvert.tm_hour % 12 ? "PM" : "AM";

        timeStr = (std::stringstream() << std::setfill('0') << ' ' <<
            hour << ':' << std::setw(2) << min << ' ' << meridiem).str();
    }

    return (includeWeekday ? dayToString(tmToConvert.tm_wday) + ", " : "") + monthToString(tmToConvert.tm_mon) + ' ' +
        std::to_string(tmToConvert.tm_mday) + ", " + std::to_string(tmToConvert.tm_year + 1900) + timeStr;
}

bool lessTmDateOnly(const tm& a, const tm& b) {
    if (a.tm_year < b.tm_year) return true;
    else if (a.tm_year > b.tm_year) return false;

    if (a.tm_mon < b.tm_mon) return true;
    else if (a.tm_mon > b.tm_mon) return false;

    if (a.tm_mday < b.tm_mday) return true;

    return false;
}

DateRange::DateRange(const tm& start, const tm& end) : start(start), end(end) {
    if (lessTmDateOnly(end, start))
        throw std::runtime_error("You cannot create a date range with the end before the start.");
}

DateRange::DateRange(const tm& time) : start(time), end(time) { }

const tm& DateRange::getStart() const { return start; }

const tm& DateRange::getEnd() const { return end; }