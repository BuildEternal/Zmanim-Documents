#include <regex>
#include <string>

#include "../web/zmanim.h"

#include "zman_html.h"

const std::string htmlTemplate = ""
#include "../html/zman.txt"
;

ZmanHtml::ZmanHtml(const Zman& zman) {
    html = std::regex_replace(htmlTemplate, std::regex(R"(\{title\})"), zman.getTitle());
    html = std::regex_replace(html, std::regex(R"(\{time\})"), zman.getZman());
};

const std::string& ZmanHtml::getHtml() const { return html; }