#pragma once
#include <ArduinoJson.h>

namespace JSON
{
    const int JSON_DOC_SIZE = 200;
    StaticJsonDocument<JSON_DOC_SIZE> doc;
    const String TEMP_TAG = String("temperature");
    const String TEMP_BASEMENT_TAG = String("temperature.basement");
    const String PRESSURE_TAG = String("pressure");
    const String HUMIDITY_TAG = String("humidity");
}

class JSONSender
{
public:

    void Serialize()
    {
        _jsonString = String();
        JSON::doc.clear();
        JSON::doc[JSON::TEMP_TAG] = _temperature;
        JSON::doc[JSON::HUMIDITY_TAG] = _humidity;
        JSON::doc[JSON::TEMP_BASEMENT_TAG] = _temperatureBasement;
        serializeJson(JSON::doc, _jsonString);
    }

    String GetJsonString() const {
        return _jsonString;
    }

    void UpdateTemperatureVal(float newVal) {
        _temperature = newVal;
    }

private:
    String _jsonString;
    float _temperature = 0.f;
    float _temperatureBasement = 0.f;
    float _humidity = 0.f;
};

namespace JSON
{
    JSONSender Sender;
}