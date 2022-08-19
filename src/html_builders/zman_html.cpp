#include <set>
#include <regex>
#include <string>

#include "../web/zmanim.h"

#include "zman_html.h"

const std::string htmlTemplate = ""
#include "../html/zman.txt"
;

const std::set<std::string> emphasized(
    {
        "CandleLighting",
        "Shkiah",
        "ShabbatEndTime",
        "LastEatingChametzTime",
        "BurnChametzTime"
    }
);

ZmanHtml::ZmanHtml(const Zman& zman) {
    html = std::regex_replace(
        htmlTemplate, std::regex(R"(\{if_emphasized\s*([\s\S]*?)\s*if_emphasized\})"),
        emphasized.find(zman.getType()) != emphasized.end() ? "$1" : "");

    html = std::regex_replace(html, std::regex(R"(\{title\})"), zman.getTitle());
    html = std::regex_replace(html, std::regex(R"(\{time\})"), zman.getZman());
};

const std::string& ZmanHtml::getHtml() const { return html; }