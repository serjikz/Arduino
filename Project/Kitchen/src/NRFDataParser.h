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

    const int DATA_ARRAY_SIZE = 7;

    float receivedData[DATA_ARRAY_SIZE];
    float jsonData[DATA_ARRAY_SIZE];

    const float HASH = 3.1415926f;
    const float EPS = 0.001f;
    const int MAX_RECEIVING_ATTEMPTS = 1000000;
    int receivingAttempts = 0; 
   
    unsigned long receivingPauseTimer = millis();
    const int RECEIVING_DELAY = 5000;

    bool DataIsValid() {
        return abs(Radio::receivedData[0] - Radio::HASH) < Radio::EPS;
    }

    bool ReadyToGetNextData() {
        return millis() - receivingPauseTimer >= RECEIVING_DELAY;
    }

    void ResetDataReceivingProcess() {
        receivingPauseTimer = millis();
        receivingAttempts = 0;
    }
}

namespace JSON {
    static const unsigned short int JSON_DOC_SIZE = 200;
    StaticJsonDocument<JSON_DOC_SIZE> doc;
    const String TEMP_TAG = String("temperature");
    const String TEMP_STREET_TAG = String("temperature.street");
    const String PRESSURE_TAG = String("pressure");
    const String HUMIDITY_TAG = String("humidity");    
}

namespace Radio {
    class NRFDataParser {
    public:
 
        void ClearReceivedData() {
            for (size_t i = 0; i < Radio::DATA_ARRAY_SIZE; i++) {
                Radio::receivedData[i] = 0.f;
            }
        }
    };

    class BalconyNRFDataParser : public NRFDataParser {
        float _temperature;
        float _temperatureStreet;
        float _humidity;
    public:

        void SaveReceivedData() {
            _temperature = Radio::receivedData[3];
            _humidity = Radio::receivedData[4];
            _temperatureStreet = Radio::receivedData[5];          
        }
        
        void PrintDebugInfo() {
            Serial.println("From Balcony[0]:(hash): " + String(Radio::receivedData[0]));
            Serial.println("From Balcony[1]:(modulePos): " + String(Radio::receivedData[1]));
            Serial.println("From Balcony[2]:(sensorsCount): " + String(Radio::receivedData[2]));
            Serial.println("From Balcony[3]:(temp): " + String(Radio::receivedData[3]));
            Serial.println("From Balcony[4]:(humidity): " + String(Radio::receivedData[4]));
            Serial.println("From Balcony[5]:(temp.street): " + String(Radio::receivedData[5]));
        }

        float GetTemperature() {
            return _temperature;
        }

        float GetHumidity() {
            return _humidity;
        }

        float GetTemperatureOnStreet() {
            return _temperatureStreet;
        }

        String GetDisplayTempBalcony() {
            return String("T balc " + String(_temperature) + String(char(223)) + String("C   "));
        }

        String GetDisplayTempStreet() {
            return String("T str  " + String(_temperatureStreet) + String(char(223)) + String("C"));
        }

        String GetDisplayHumidityBalcony() {
            return String("H balc  " + String(_humidity) + String("%"));
        }
    };
}
