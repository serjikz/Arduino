#include "esphome.h"
#include <ArduinoJson.h>

class MegaUARTConnector : public PollingComponent, public UARTDevice, public sensor::Sensor {
public:
    MegaUARTConnector(UARTComponent *parent) 
        : PollingComponent(kDelay), 
          UARTDevice(parent)
    {
        bmp280TempSensor = new Sensor();
        bmp280PressureSensor = new Sensor();
    }

    void setup() override {

    }
  
    void loop() override {
        while (available()) {
            char ch = read();
            if (ch == '^') {
                ch = read();
                while (ch != '#') {
                    _dataBuf += ch;
                    ch = read();
                }
            } else {
                _temp = -1;
                _pressure = -1;
                return;
            }
                DynamicJsonBuffer doc(kJsonDocSize);
                JsonObject& root = doc.parseObject(_dataBuf);
                if (root.size() > 0) {
                    _temp = root["temp"].as<float>();
                    _pressure = root["pressure"].as<float>();
                    //_temp = -2;
                    //_pressure = -2;
                    _dataBuf = "";
                } else {
                   _temp = -3;
                   _pressure = -3;
                   _dataBuf = "";
                }
        }
    }
  
    void update() override {
        bmp280TempSensor->publish_state(_temp);
        bmp280PressureSensor->publish_state(_pressure);
    }

    Sensor* bmp280TempSensor;
    Sensor* bmp280PressureSensor;

private:
    String _dataBuf;
    float _temp;
    float _pressure;
    static const int kDelay = 1000;
    static const int kJsonDocSize = 200;
    
  
};
