#ifndef WEATHER_STATE_H
#define WEATHER_STATE_H

#include <sstream>
#include <ArduinoJson.h>

#ifndef FORECAST_DAYS
#define FORECAST_DAYS 3
#endif
#ifndef FORECAST_HOURS
#define FORECAST_HOURS 12
#endif 

struct ForecastDay
{
    ForecastDay()
    {
        this->valid = false;
        this->condition = "";
    }

    bool valid;
    int precip_probability;
    int precipitation;
    int temp_low;
    int temp_high;
    std::string condition;
    std::string day;
};


struct ForecastHour
{
    ForecastHour()
    {
        this->valid = false;
        this->condition = "";
    }
    
    bool valid;
    int temp;
    std::string condition;
    float precip;
    int hour;
};

struct ForecastDay *weatherForecastDaily = new ForecastDay[FORECAST_DAYS];
struct ForecastHour *weatherForecastHourly = new ForecastHour[FORECAST_HOURS];
bool showHourly = false;

void parseDailyForecast(std::string rawJson)
{
    // parse weather from AccuWeather
    DynamicJsonDocument doc(16384);
    std::istringstream st(rawJson);
    auto error = deserializeJson(doc, st /* updated value */);
    if (!error) {
        JsonArray array = doc.as<JsonArray>();
        int i = 0;
        time_t t = std::time(NULL);
        struct tm *ts = localtime(&t);
        int wday = ts->tm_wday;
        for(JsonVariant v : array) 
        {
            JsonObject obj = v.as<JsonObject>();
            if (i >= FORECAST_DAYS) break;
            weatherForecastDaily[i].temp_low = obj["templow"].as<int>();
            weatherForecastDaily[i].temp_high = obj["temperature"].as<int>();
            weatherForecastDaily[i].precip_probability = obj["precipitation_probability"].as<float>();
            weatherForecastDaily[i].precipitation = obj["precipitation"].as<float>();
            weatherForecastDaily[i].condition = obj["condition"].as<std::string>();
            weatherForecastDaily[i].valid = true;
            // this hack assumes that the first forecast is for today, and they increment one per day
            // that isn't true for all forecast arrays on HomeAssistant, but works for AccuWeather
            switch ((wday + i) % 7)
            {
                case 0:  weatherForecastDaily[i].day = "Sun"; break;
                case 1:  weatherForecastDaily[i].day = "Mon"; break;
                case 2:  weatherForecastDaily[i].day = "Tue"; break;
                case 3:  weatherForecastDaily[i].day = "Wed"; break;
                case 4:  weatherForecastDaily[i].day = "Thu"; break;
                case 5:  weatherForecastDaily[i].day = "Fri"; break;
                case 6:  weatherForecastDaily[i].day = "Sat"; break;
                default: weatherForecastDaily[i].day = "???"; break;
            }
            ESP_LOGD("lilygo", "%i: %i->%i %i %s, %s, %i mm", i, weatherForecastDaily[i].temp_low, weatherForecastDaily[i].temp_high, weatherForecastDaily[i].precip_probability, weatherForecastDaily[i].day.c_str(), weatherForecastDaily[i].condition.c_str(), weatherForecastDaily[i].precipitation);
            i++;
        }
    }
    else
    {
        ESP_LOGW("lilygo", "deserializeJson returned %s", error.c_str());
        char next[100];
        st.seekg(-20, st.cur);
        st.getline(next, 100);
        ESP_LOGW("lilygo", "error in 20 chars: %s", next);
    }
}


#endif // WEATHER_STATE_H