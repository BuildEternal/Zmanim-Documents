#include <iostream>
#include <ctime>

using namespace std;

/**
 * @brief Returns if the given `tm` is valid.
 *
 * @param check The `tm` to be checked
 * @return `true` if the `tm` correctly represents a date and is after the Unix epoch, and `false` if the `tm` does not
 * correctly represent a date or is after the Unix epoch.
 */
bool valid_dmy(const tm &check)
{
    auto copy = check;

    time_t as_time_t = mktime(addressof(copy));

    if (as_time_t == -1)
        return false;

    copy = *localtime(addressof(as_time_t));

    return check.tm_mday == copy.tm_mday &&
           check.tm_mon == copy.tm_mon &&
           check.tm_year == copy.tm_year;
}