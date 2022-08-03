#pragma once

#include <ctime>

tm normalizeTm(tm toNormalize);

bool valid_dmy(const tm& check);

std::string monthName(int month);

std::string monthToString(int month);

std::string dateToSuffix(int date);

tm tmFromString(std::string str, bool basicString = false);

std::string tmToBasicDateString(tm tmToConvert);

std::string tmToDateString(tm tmToConvert);

bool lessTmDateOnly(const tm& a, const tm& b);

bool lessTm(const tm& a, const tm& b);