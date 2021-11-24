#pragma once
#include <ArduinoJson.hpp>
#include <ArduinoJson.h>    

namespace Radio {
    const int PIN_CE = 9;
    const int PIN_CSN = 10;
    const uint8_t CHANNEL = 0x6f;

    const uint32_t PIPE_0_ADDR = 111156789;
    const uint64_t PIPE_1_ADDR = 0xC4C5C6C7F1;
    const uint64_t PIPE_2_ADDR = 0xC4C5C6C7F2;

    enum ModulePosition {
        Balcony = 0,
        Hall = 1,
        Garage
    };

    enum SensorsCount {
        One = 1,
        Two = 2,
        Three,
        Four,
        Five,
        Six
    };

    const int DATA_ARRAY_SIZE = 5;

    float receivedData[DATA_ARRAY_SIZE];
    float jsonData[DATA_ARRAY_SIZE];

    const float HASH = 3.1415926f;
    const float EPS = 0.001f;
    const unsigned short int MAX_RECEIVING_ATTEMPTS = 60000;
    int receivingAttempts = 0; 

    bool DataIsValid() {
        return abs(Radio::receivedData[0] - Radio::HASH) < Radio::EPS;
    }
}

namespace JSON {
    static const unsigned short int JSON_DOC_SIZE = 200;
    StaticJsonDocument<JSON_DOC_SIZE> doc;
    const String TEMP_TAG = String("temperature");
    const String PRESSURE_TAG = String("pressure");
    const String HUMIDITY_TAG = String("humidity");    
}

namespace Radio {
    class NRFDataParser {
    public:

        virtual void SendNewJsonToESP() = 0;
        virtual void SendOldJsonToESP() = 0;
 
        void ClearReceivedData() {
            for (size_t i = 0; i < Radio::DATA_ARRAY_SIZE; i++) {
                Radio::receivedData[i] = 0.f;
            }
        }
    };

    class BalconyNRFDataParser : public NRFDataParser {
        float _temperature;
        float _humidity;
    public:

        void SendNewJsonToESP() {
            _temperature = Radio::receivedData[3];
            _humidity = Radio::receivedData[4];
            JSON::doc.clear();
            JSON::doc[JSON::TEMP_TAG] = _temperature;
            JSON::doc[JSON::PRESSURE_TAG] = -1;
            JSON::doc[JSON::HUMIDITY_TAG] = _humidity;
            serializeJson(JSON::doc, Serial);
            Serial.write('\n');
        }

        void SendOldJsonToESP() {
            JSON::doc.clear();
            JSON::doc[JSON::TEMP_TAG] = _temperature;
            JSON::doc[JSON::PRESSURE_TAG] = -1;
            JSON::doc[JSON::HUMIDITY_TAG] = _humidity;
            serializeJson(JSON::doc, Serial);
            Serial.write('\n');
        }
    };
}
