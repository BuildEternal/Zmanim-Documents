#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>

#include "zman_info.h"

std::string ZmanInfo::htmlTemplate = ""
#include "../html/zman.txt"
;

ZmanInfo::ZmanInfo(std::string newTitle, std::string newZman) : title(newTitle), zman(newZman) {
    html = htmlTemplate;

    html = std::regex_replace(
        html,
        std::regex(R"(\{title\})"),
        newTitle
    );

    html = std::regex_replace(
        html,
        std::regex(R"(\{time\})"),
        newZman
    );
};

std::string ZmanInfo::getTitle() const { return title; }

std::string ZmanInfo::getZman() const { return zman; }

std::string ZmanInfo::getHtml() const { return html; }