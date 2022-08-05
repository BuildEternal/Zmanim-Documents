#include <iostream>
#include <string>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>
#include <nlohmann/json.hpp>

#include "zip_codes.h"

using nlohmann::json;

ZipCodeInfo::ZipCodeInfo() { };

ZipCodeInfo::ZipCodeInfo(const std::string& name, const std::string& zipCode, double latitude, double longitude) :
    name(name), zipCode(zipCode), latitude(latitude), longitude(longitude) { }

const std::string& ZipCodeInfo::getName() const { return name; }
const std::string& ZipCodeInfo::getZipCode() const { return zipCode; }
double ZipCodeInfo::getLatitude() const { return latitude; }
double ZipCodeInfo::getLongitude() const { return longitude; }

std::string getZipCodeError = "There was an error getting info for the given zip code.";

ZipCodeInfo getZipCodeInfo(const std::string& query) {
    httplib::Client hebcalCli("https://www.hebcal.com");

    auto res = hebcalCli.Get("/complete.php", httplib::Params(
        {
            { "q", query }
        }
    ), httplib::Headers());

    if (res->status == 404) return ZipCodeInfo();
    else if (res->status != 200) throw std::runtime_error(getZipCodeError);

    json::array_t resultsJsonArray = json::parse(res->body);

    json::object_t chosenResultJsonObject;

    bool found = false;

    for (json::object_t resultJsonObject : resultsJsonArray) {
        if (resultJsonObject["geo"].get<std::string>() == "zip") {
            chosenResultJsonObject = resultJsonObject;
            found = true;
            break;
        }
    }

    if (!found) return ZipCodeInfo();

    std::string zip = chosenResultJsonObject["id"].get<std::string>();

    if (chosenResultJsonObject["latitude"].is_null()) {
        res = hebcalCli.Get("/complete.php", httplib::Params(
            {
                { "q", zip }
            }
        ), httplib::Headers());

        if (res->status != 200) throw std::runtime_error(getZipCodeError);

        resultsJsonArray = json::parse(res->body);

        chosenResultJsonObject = resultsJsonArray[0];
    }

    std::string name = chosenResultJsonObject["value"].get<std::string>();
    double latitude = chosenResultJsonObject["latitude"].get<double>();
    double longitude = chosenResultJsonObject["longitude"].get<double>();

    return ZipCodeInfo(name, zip, latitude, longitude);
}