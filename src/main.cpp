#include <fstream>
#include <iostream>
#include <vector>

#include <nlohmann/json.hpp>

#include "parse/parse_date.h"
#include "get_zmanim.h"
#include "timespan.h"
#include "zmanim/document_info.h"

using json = nlohmann::json;

int main(int argc, char* argv[])
{
    std::vector<std::string> args;

    if (argc > 1) {
        args.assign(argv + 1, argv + argc);
    }

    time_t date;

    try {
        tm givenDate = parseDate(args);

        int month = givenDate.tm_mon + 1;
        int day = givenDate.tm_mday;
        int year = givenDate.tm_year + 1900;

        // std::cout << month << '/' << day << '/' << year << " is the date you have inputted. It is valid." << std::endl;

        Timespan timespan(givenDate);
        // std::cout << 1 << std::endl;
        // std::vector<DayInfo> b;
        // b.push_back(DayInfo(givenDate, "", "", { ZmanInfo("", "") }));
        // std::cout << 2 << std::endl;
        // DocumentInfo a(b, "", "", "");
        // std::cout << 3 << std::endl;
        DocumentInfo zmanimJson = get_zmanim(timespan, 2, 2135);

        std::ofstream outputFile("zmanim.html");

        outputFile << zmanimJson.getHtml();

        outputFile.close();
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}