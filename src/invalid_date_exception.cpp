#include <iostream>

#include "invalid_date_exception.h"

InvalidDateException::InvalidDateException(std::string date) : std::runtime_error(
    "The date '" + date + "' is invalid. Make sure it is a valid date, is formatted as 'dd/mm/yy' or 'dd/mm/yyyy', "
    "and is within a reasonable time frame."
) {}