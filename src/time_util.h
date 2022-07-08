#pragma once

#include <ctime>

tm normalizeTm(tm toNormalize);

bool valid_dmy(const tm& check);

std::string monthName(int month);

std::string dateSuffix(int day);

class DateToString {
    static std::string months[12];
    static std::string dateSuffixes[10];
public:
    static std::string monthToString(int month);
    static std::string dateToSuffix(int date);
};