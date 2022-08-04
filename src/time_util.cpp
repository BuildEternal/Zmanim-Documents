#include <ctime>
#include <iostream>
#include <sstream>
#include <regex>
#include <iomanip>

#include "time_util.h"

using namespace std;

tm normalizeTm(tm toNormalize) {
    time_t asTimeT = mktime(&toNormalize);

    if (asTimeT == -1)
        throw invalid_argument("A time was invalid.");

    return *localtime(&asTimeT);
}

/**
 * @brief Returns if the given `tm` is valid.
 *
 * @param check The `tm` to be checked
 * @return `true` if the `tm` correctly represents a date and is after the Unix epoch, and `false` if the `tm` does not
 * correctly represent a date or is after the Unix epoch.
 */
bool valid_dmy(const tm& check) {
    auto copy = check;

    try {
        copy = normalizeTm(copy);
    }
    catch (invalid_argument) {
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

std::string dateToSuffix(int date) {
    if (11 <= date && date <= 13) return "th";

    switch (date % 10) {
    case 1: return "st";
    case 2: return "nd";
    case 3: return "rd";
    default: return "th";
    }
}

tm tmFromString(std::string str, bool basicString) {
    tm newTm;

    if (!basicString) {
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
std::string tmToDateString(tm tmToConvert) {
    return (std::stringstream() << std::setfill('0') <<
        std::setw(4) << tmToConvert.tm_year + 1900 << '-' <<
        std::setw(2) << tmToConvert.tm_mon + 1 << '-' <<
        std::setw(2) << tmToConvert.tm_mday).str();
}

bool lessTmDateOnly(const tm& a, const tm& b) {
    if (a.tm_year < b.tm_year) return true;
    else if (a.tm_year > b.tm_year) return false;

    if (a.tm_mon < b.tm_mon) return true;
    else if (a.tm_mon > b.tm_mon) return false;

    if (a.tm_mday < b.tm_mday) return true;

    return false;
}

bool lessTm(const tm& a, const tm& b) {
    if (lessTmDateOnly(a, b)) return true;
    else if (lessTmDateOnly(b, a)) return false;

    if (a.tm_hour < b.tm_hour) return true;
    else if (a.tm_hour > b.tm_hour) return false;

    if (a.tm_min < b.tm_min) return true;
    else if (a.tm_min > b.tm_min) return false;

    if (a.tm_sec < b.tm_sec) return true;

    return false;
}