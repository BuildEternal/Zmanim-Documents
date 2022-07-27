#include <fstream>
#include <iostream>
#include <vector>

#include <nlohmann/json.hpp>

#include "parse/parse_date.h"
#include "get_zmanim.h"
#include "timespan.h"
#include "zmanim/document_info.h"
#include "time_util.h"

using json = nlohmann::json;

int main(int argc, char* argv[])
{
    std::vector<std::string> args;

    if (argc > 1) {
        args.assign(argv + 1, argv + argc);
    }

    time_t date;

    tm givenDate = parseDate(args);
    tm nextDate = givenDate;
    nextDate.tm_mday += 4;
    nextDate = normalizeTm(nextDate);

    Timespan timespan(givenDate, nextDate);

    DocumentInfo zmanimJson = get_zmanim(timespan, 2, 2135);

    std::string css = ""
        #include "html/styles.txt"
    ;

    std::ofstream outputHtml("zmanim.html");
    std::ofstream outputCss("styles.css");

    outputHtml << zmanimJson.getHtml();
    outputCss << css;

    outputHtml.close();
    outputCss.close();

    return 0;
}