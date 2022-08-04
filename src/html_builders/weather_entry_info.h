#pragma once

#include <string>

class WeatherEntryInfo {
    static std::string htmlTemplate;

    bool day;
    int temp;
    std::string tempUnit;
    std::string windDirection;
    std::string windSpeed;
    std::string details;
    std::string html;
public:
    WeatherEntryInfo(
        bool newDay, int newTemp, std::string newTempUnit, std::string newWindDirection,
        std::string newWindSpeed, std::string newDetails
    );
    // Empty constructor will have no HTML.
    WeatherEntryInfo();

    bool getDay() const;
    int getTemp() const;
    std::string getTempUnit() const;
    std::string getWindDirection() const;
    std::string getWindSpeed() const;
    std::string getDetails() const;
    std::string getHtml() const;
};