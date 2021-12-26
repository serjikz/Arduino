#pragma once
#include "Sensor.h"
#include <OneWire.h>
#include <DallasTemperature.h>

// 5V 
OneWire DS18B20Wire(PIND2);
DallasTemperature DS18B20Main(&DS18B20Wire);

class DS18B20Sensor : public Sensor {
    typedef Sensor Super;

public:

    DS18B20Sensor(String id)
        : Super(id)
        , _ds18b20(&DS18B20Main)
    {}

    float GetTemperature() {
        if (_ds18b20 && _inited) {
            _ds18b20->requestTemperatures();
            float newVal = _ds18b20->getTempCByIndex(0);
            if (abs(newVal) < MAX) {
                _lastVal = newVal;
            }            
        }
        return _lastVal;
    }

    virtual void Init() {
        if (!_ds18b20) {
            return;
        }
        _ds18b20->begin();
        delay(INIT_DELAY);
        if (_ds18b20->getDeviceCount() > 0) {
            _inited = true;
            Serial.println("Inited: " + GetID());
        }
        else {
            _inited = false;
            Serial.println("Not inited " + GetID());
        }
    }

private:
    DallasTemperature* _ds18b20;
    const size_t INIT_DELAY = 100;
    const float MAX = 100;
    float _lastVal = 0.f;
};

namespace Sensors {
    DS18B20Sensor DS18B20("DS18B20.Balcony.Street");
}
