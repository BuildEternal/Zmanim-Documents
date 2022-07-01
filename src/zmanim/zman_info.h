#pragma once

#include <iostream>

class ZmanInfo {
    std::string title;
    std::string zman;

public:
    ZmanInfo(std::string title, std::string zman);

    std::string getTitle() const;
    std::string getZman() const;
};