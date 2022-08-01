#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <sstream>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "cpp-httplib/httplib.h"
#include "nlohmann/json.hpp"

#include "args.h"
#include <get_zmanim.h>
#include "parse_date.h"
#include <time_util.h>
#include <timespan.h>
#include <zmanim/document_info.h>

using json = nlohmann::json;

void parseGet(std::vector<std::string>& args);
void parseGetZmanim(std::vector<std::string>& args);

void parseCommand(std::vector<std::string>& args) {
    std::string cmd = pullArg(args);

    if (cmd == "get") {
        parseGet(args);
    }
    else {
        throw std::invalid_argument("There is no '" + cmd + "' command.");
    }
}

void parseGet(std::vector<std::string>& args) {
    std::string what = pullArg(args);

    if (what == "zmanim" || what == "times") {
        parseGetZmanim(args);
    }
    else {
        throw std::invalid_argument("The meaning of '" + what + "' in this context is unknown.");
    }
}

void parseGetZmanim(std::vector<std::string>& args) {
    std::string forPrep = pullArg(args);

    if (forPrep != "for") throw std::invalid_argument(
        "The meaning of '" + forPrep + "' in this context is unknown."
    );

    Timespan dateRange = parseTimespan(args);

    std::string inPrep = pullArg(args);

    if (inPrep != "in") throw std::invalid_argument(
        "The meaning of '" + inPrep + "' in this context is unknown."
    );

    std::string location = argsVectorToString(args, " ");

    if (location == "") throw std::invalid_argument(
        "No location was provided."
    );

    httplib::Client cli("https://www.hebcal.com");

    auto res = cli.Get("/complete.php", httplib::Params(
        {
            {"q", location}
        }
    ), httplib::Headers());

    const std::string notFoundError = "The zip code '" + location +
        "' couldn't be found. Make sure it is spelled correctly, or try rephrasing it or being more specific.";

    if (res->status != 200) throw std::runtime_error(notFoundError);

    json::array_t locationsJson = json::parse(res->body);

    // TODO: Make this go through all given options

    json::object_t chosenLocationJson = locationsJson[0];

    if (chosenLocationJson["geo"].get<std::string>() != "zip") throw std::runtime_error(notFoundError);

    std::string locationTitle = chosenLocationJson["value"].get<std::string>();

    if (locationsJson.size() > 1) {
        std::cout
            << "Are you trying to get zmanim for " + locationTitle + "? (yes/no)\n";

        std::string c;

        while (true) {
            std::cin >> c;

            if (c == "y" || c == "yes") {
                break;
            }
            else if (c == "n" || c == "no") {
                std::cout <<
                    "Your location was too vague. Try writing this command again but with a more specific location.\n";
                return;
            }
        }
    }

    std::string zip = chosenLocationJson["id"].get<std::string>();

    tm startDate = dateRange.getStart();
    tm endDate = dateRange.getEnd();

    std::string beginDateStr = (std::stringstream() << startDate.tm_mon + 1 << '/' << startDate.tm_mday << '/' <<
        startDate.tm_year + 1900).str();
    std::string endDateStr = (std::stringstream() << endDate.tm_mon + 1 << '/' << endDate.tm_mday << '/' <<
        endDate.tm_year + 1900).str();
    std::string dateRangeStr = beginDateStr == endDateStr ? beginDateStr : beginDateStr + " to " + endDateStr;

    std::cout << "Generating zmanim documents for " << dateRangeStr << " in " << locationTitle << "...\n";

    DocumentInfo zmanimJson = get_zmanim(dateRange, "2", zip);

    std::ofstream outputHtml("zmanim.html");
    std::ofstream outputCss("styles.css");

    outputHtml << zmanimJson.getHtml();
    outputCss << ""
#include "html/styles.txt"
        ;

    outputHtml.close();
    outputCss.close();
}