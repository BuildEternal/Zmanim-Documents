#pragma once

#include <ctime>

tm normalizeTm(tm toNormalize);

bool valid_dmy(const tm& check);

std::string monthName(int month);

std::string monthToString(int month);

std::string dateToSuffix(int date);

bool lessTm(tm a, tm b);