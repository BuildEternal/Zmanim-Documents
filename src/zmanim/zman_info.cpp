#include <iostream>

#include "zman_info.h"

ZmanInfo::ZmanInfo(std::string title, std::string zman) : title(title), zman(zman) {};

std::string ZmanInfo::getTitle() const { return title; }

std::string ZmanInfo::getZman() const { return zman; }