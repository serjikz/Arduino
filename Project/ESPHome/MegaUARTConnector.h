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
        aht10HumiditySensor = new Sensor();
    }

    void setup() override {

    }
  
    void loop() override {
        while (available()) {
            char ch = read();
            while (ch != '\n') {
                _dataBuf += ch;
                ch = read();
            }
            DynamicJsonBuffer doc(kJsonDocSize);
            JsonObject& root = doc.parseObject(_dataBuf);
            if (root.size() > 0) {
                _temp = root["temp"].as<float>();
                _pressure = root["pressure"].as<float>();
                _humidity = root["humidity"].as<float>();
                _dataBuf = "";
            } else {
                _dataBuf = "";
            }
        }
    }
  
    void update() override {
        bmp280TempSensor->publish_state(_temp);
        bmp280PressureSensor->publish_state(_pressure);
        aht10HumiditySensor->publish_state(_humidity);
    }

    Sensor* bmp280TempSensor;
    Sensor* bmp280PressureSensor;
    Sensor* aht10HumiditySensor;

private:
    String _dataBuf;
    float _temp;
    float _pressure;
    float _humidity;
    static const int kDelay = 1000;
    static const int kJsonDocSize = 200;
    
  
};
