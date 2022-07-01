#include <iostream>
#include <ctime>

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