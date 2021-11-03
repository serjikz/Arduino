#pragma once
#include "Sensor.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_AHT10.h>

Adafruit_AHT10 AHT10Adafruit;

class AHT10Sensor : public Sensor {
    typedef Sensor Super;

public:

    AHT10Sensor(String id)
        :Super(id)
        , _ahtMain(&AHT10Adafruit)
    {}

    String GetTemperature() {
        if (_aht_temp && _inited) {
            sensors_event_t temp;
            _aht_temp->getEvent(&temp);
            return "Temperature = " + String(temp.temperature) + " *C";
        }
        else {
            return Super::GetSensorNotInitializedMsg();
        }
    }

    String GetHumidity() {
        if (_aht_humidity && _inited) {
            sensors_event_t humidity;
            _aht_temp->getEvent(&humidity);
            return "Humidity = " + String(humidity.relative_humidity) + " %";
        }
        else {
            return Super::GetSensorNotInitializedMsg();
        }
    }

    virtual void Init() {
        if (!_ahtMain) {
            return;
        }
        if (!_ahtMain->begin()) {
            Serial.println("Not inited " + GetID());
            _inited = false;
        }
        else {
            Serial.println("Inited " + GetID());
            _aht_humidity = _ahtMain->getHumiditySensor();
            _aht_temp = _ahtMain->getTemperatureSensor();
            _inited = true;
        }
    }

private:
    Adafruit_AHT10* _ahtMain;
    Adafruit_Sensor* _aht_humidity;
    Adafruit_Sensor* _aht_temp;
};

namespace Sensors {
    AHT10Sensor AHT10("AHT10");
}
