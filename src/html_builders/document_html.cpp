#include <regex>
#include <string>
#include <vector>
#include <stdexcept>

#include "../web/forecast.h"
#include "../web/hebrew_dates.h"
#include "../web/zmanim.h"
#include "day_html.h"
#include "document_html.h"

const std::string htmlTemplate = ""
#include "../html/document.txt"
;

DocumentHtml::DocumentHtml(const ZmanimData& zmanim, const HebrewDateMap& hebrewDates, const Forecast& forecast) {
    std::string daysHtml;

    for (const auto& [date, day] : zmanim.getDayData()) {
        std::string hebrewDate;
        try { hebrewDate = hebrewDates.at(date); }
        catch (const std::out_of_range& e) {}

        ForecastEntry dayForecast;
        ForecastEntry nightForecast;

        if (!forecast.isEmpty()) {
            try { dayForecast = forecast.getForecastInfo().at(date).at(true); }
            catch (const std::out_of_range& e) {}

            try { nightForecast = forecast.getForecastInfo().at(date).at(false); }
            catch (const std::out_of_range& e) {}
        }

        daysHtml += DayHtml(date, hebrewDate, day, dayForecast, nightForecast).getHtml();
    }

    html = std::regex_replace(htmlTemplate, std::regex(R"(\{location_name\})"), zmanim.getLocationName());
    html = std::regex_replace(html, std::regex(R"(\{days\})"), daysHtml);
}

const std::string& DocumentHtml::getHtml() const { return html; }