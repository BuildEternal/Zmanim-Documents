#pragma once

#include <iostream>

class ZmanInfo {
    static std::string htmlTemplate;

    std::string title;
    std::string zman;
    std::string html;
public:
    ZmanInfo(std::string newTitle, std::string newZman);

    std::string getTitle() const;
    std::string getZman() const;
    std::string getHtml() const;
};