#pragma once

#include <string>

#include "../web/zmanim.h"

class ZmanHtml {
    std::string html;
public:
    ZmanHtml(const Zman& zman);

    const std::string& getHtml() const;
};