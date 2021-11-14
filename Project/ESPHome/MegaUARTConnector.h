#include "esphome.h"

class MegaUARTConnector : public PollingComponent, public UARTDevice, public sensor::Sensor {
public:
    MegaUARTConnector(UARTComponent *parent) 
        : PollingComponent(kDelay), 
          UARTDevice(parent)
    {}

    void setup() override {
    
    }
  
    void loop() override {
        while (available()) {
            char ch = read();
            if (ch != '#') {
                _dataBuf += ch;
            } else {
                _data = _dataBuf;
                _dataBuf = "";
                return;
            }
        }
    }
  
    void update() override {
        publish_state(_data.toFloat());
    }

private:
    String _data;
    String _dataBuf;
    static const int kDelay = 1000;
  
};
