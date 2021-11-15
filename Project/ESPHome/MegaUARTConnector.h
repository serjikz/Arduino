#include "esphome.h"

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
            if (ch == '/') {
                _temp = _dataBuf;
                _dataBuf = "";
                ch = read();
            }
            if (ch != '#') {
                _dataBuf += ch;
            } else {
                _pressure = _dataBuf;
                _dataBuf = "";
                return;
            }
        }
    }
  
    void update() override {
        //publish_state(_data.toFloat());
        bmp280TempSensor->publish_state(_temp.toFloat());
        bmp280PressureSensor->publish_state(_pressure.toFloat());
    }

    Sensor* bmp280TempSensor;
    Sensor* bmp280PressureSensor;

private:
    String _data;
    String _dataBuf;
    String _temp;
    String _pressure;
    static const int kDelay = 1000;

  
};
