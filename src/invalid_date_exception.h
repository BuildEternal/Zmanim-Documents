#pragma once

#include <iostream>

class InvalidDateException : public std::runtime_error
{
public:
    InvalidDateException(std::string date);
};