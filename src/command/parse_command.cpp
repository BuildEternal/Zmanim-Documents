#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "../html_builders/document_html.h"
#include "../web/hebrew_dates.h"
#include "../web/zip_codes.h"
#include "../web/forecast.h"
#include "../web/zmanim.h"
#include "args_util.h"
#include "parse_date.h"
#include "../time_util.h"

void parseGetZmanim(std::vector<std::string>& args) {
    std::string forPrep = pullArg(args);

    if (forPrep == "") throw std::invalid_argument("'for' expected after 'get'.");
    else if (forPrep != "for") throw std::invalid_argument("'for' expected after 'get', not '" + forPrep + "'.");

    DateRange dateRange = parseTimespan(args);

    const tm& startDate = dateRange.getStart();
    const tm& endDate = dateRange.getEnd();

    std::string inPrep = pullArg(args);

    if (inPrep == "") throw std::invalid_argument("'in' expected after the date range.");
    if (inPrep != "in") throw std::invalid_argument("'in' expected after the date range, not '" + inPrep + "'.");

    std::string location = argsVectorToString(args, " ");

    if (location == "") throw std::invalid_argument("Expected a zip code after 'in'.");

    ZipCodeInfo zipCodeInfo = getZipCodeInfo(location);

    const std::string& zipCode = zipCodeInfo.getZipCode();

    if (zipCode == "") throw std::invalid_argument("The zip code '" + location +
        "' couldn't be found. Make sure it is spelled correctly, or try rephrasing it or being more specific.");

    const std::string& zipCodeName = zipCodeInfo.getName();

    double latitude = zipCodeInfo.getLatitude();
    double longitude = zipCodeInfo.getLongitude();

    std::string startDateStr = tmToPrettyDateString(startDate);
    std::string endDateStr = tmToPrettyDateString(endDate);
    std::string dateRangeStr = startDateStr == endDateStr ? startDateStr : startDateStr + " to " + endDateStr;

    std::cout << "Generating zmanim documents for " << dateRangeStr << " in " << zipCodeName << "...\n";

    std::cout << "  - Getting zmanim...\n";

    std::unique_ptr<ZmanimData> zmanim;

    try {
        zmanim = std::make_unique<ZmanimData>(getZmanim(dateRange, zipCode));
    }
    catch (const std::exception& e) {
        throw std::runtime_error(std::string("An error occurred getting zmanim: ") + e.what() + "\n");
    }

    std::cout << "  - Getting Hebrew dates...\n";

    HebrewDateMap hebrewDates(&lessTmDateOnly);

    try { hebrewDates = getHebrewDates(dateRange); }
    catch (const std::exception& e) {
        std::cout << "An error occurred getting Hebrew dates: " << e.what() <<
            "\nThe document will not contain them.";
    }

    std::cout << "  - Getting the weather forecast...\n";

    Forecast forecast;

    try { forecast = getForecast(latitude, longitude); }
    catch (const std::exception& e) {
        std::cout << "An error occurred getting forecasts: " << e.what() <<
            "\nThe document will not contain them.";
    }

    std::cout << "  - Generating document...\n";

    DocumentHtml document = DocumentHtml(*zmanim, hebrewDates, forecast);

    std::ofstream outputHtml("zmanim.html");
    std::ofstream outputCss("styles.css");

    outputHtml << document.getHtml();
    outputCss << ""
#include "html/styles.txt"
        ;

    outputHtml.close();
    outputCss.close();

    std::cout << "Document generated.\n";
}

void parseGetHelp(std::vector<std::string>& args) {
    std::cout << "\nEnter commands in lowercase.\n"
        "\n"
        "get\n"
        "    help: get this help message.\n"
        "    zmanim/times for (span of dates) in (zip code): generate a zmanim document with the given info.\n"
        "\n"
        "A span of dates can be a date, '(date) to (date)', or 'next shabbos/shabbat'.\n"
        "A date is in the format [M]M/[D]D/[[YY]Y]Y. You may encounter issues if the date is too far in the past "
        "or future.\n"
        "A zip code may also be a place name if that place corresponds to a zip code.\n\n";
}

void parseGet(std::vector<std::string>& args) {
    std::string what = pullArg(args);

    if (what == "zmanim" || what == "times") parseGetZmanim(args);
    else if (what == "help") parseGetHelp(args);
    else if (what == "") throw std::invalid_argument("Expected something after 'get'.");
    else throw std::invalid_argument("The meaning of '" + what + "' after get is unknown.");
}

void parseCommand(std::vector<std::string>& args) {
    std::string cmd = pullArg(args);

    if (cmd == "get") parseGet(args);
    else if (cmd == "") throw std::invalid_argument("Expected a command.");
    else throw std::invalid_argument("There is no '" + cmd + "' command.");
}