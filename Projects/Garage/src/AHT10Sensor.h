#pragma once
#include "Sensor.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_AHT10.h>

class AHT10Sensor : public Sensor {
    typedef Sensor Super;

public:

    AHT10Sensor(Adafruit_AHT10* ahtSensor, String id)
        : Super(id)
        , _ahtMain(ahtSensor)
    {}

    String GetTemperature() {
        if (_aht_temp && _inited) {
            sensors_event_t temp;
            _aht_temp->getEvent(&temp);
            return String(temp.temperature);
        }
        else {
            return Super::GetSensorNotInitializedMsg();
        }
    }

    String GetHumidity() {
        if (_aht_humidity && _inited) {
            sensors_event_t humidity;
            _aht_humidity->getEvent(&humidity);
            return String(humidity.relative_humidity);
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

