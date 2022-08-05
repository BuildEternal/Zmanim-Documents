#pragma once

#include <string>

class ZipCodeInfo {
    std::string name;
    std::string zipCode;
    double latitude;
    double longitude;
public:
    ZipCodeInfo();
    ZipCodeInfo(const std::string& name, const std::string& zipCode, double latitude, double longitude);

    const std::string& getName() const;
    const std::string& getZipCode() const;
    double getLatitude() const;
    double getLongitude() const;
};

ZipCodeInfo getZipCodeInfo(const std::string& query);