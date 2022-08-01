#include <iostream>
#include <ctime>

#include "time_util.h"

using namespace std;

tm normalizeTm(tm toNormalize) {
    time_t asTimeT = mktime(&toNormalize);

    if (asTimeT == -1)
        throw invalid_argument("A time was invalid.");

    return *localtime(&asTimeT);
}

// TODO: eliminate this yucky method

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
    }

    return "";
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

bool lessTm(tm a, tm b) {
    if (a.tm_year < b.tm_year) return true;
    else if (a.tm_year > b.tm_year) return false;

    if (a.tm_mon < b.tm_mon) return true;
    else if (a.tm_mon > b.tm_mon) return false;

    if (a.tm_mday < b.tm_mday) return true;
    else if (a.tm_mday > b.tm_mday) return false;

    if (a.tm_hour < b.tm_hour) return true;
    else if (a.tm_hour > b.tm_hour) return false;

    if (a.tm_min < b.tm_min) return true;

    return false;
}