#include "esphome.h"
#include <ArduinoJson.h>

class MegaUARTConnector : public PollingComponent, public UARTDevice, public Sensor {
public:
   
    MegaUARTConnector(UARTComponent *parent) 
        : PollingComponent(DELAY), 
          UARTDevice(parent)
    {
        aht10TempSensor = new Sensor();
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
            DynamicJsonBuffer doc(JSON_DOC_SIZE);
            JsonObject& root = doc.parseObject(_dataBuf);
            if (root.size() > 0) {
                _temp = root[TEMP_TAG].as<float>();
                _humidity = root[HUMIDITY_TAG].as<float>();
                _dataBuf = "";
            } else {
                _dataBuf = "";
            }
        }
    }
  
    void update() override {
        aht10TempSensor->publish_state(_temp);
        aht10HumiditySensor->publish_state(_humidity);
    }

    Sensor* aht10TempSensor;
    Sensor* aht10HumiditySensor;

private:
    String _dataBuf;
    float _temp;
    float _humidity;
    static const int DELAY = 1000;
    static const int JSON_DOC_SIZE = 200;
    
    const String TEMP_TAG = String("temperature");
    const String PRESSURE_TAG = String("pressure");
    const String HUMIDITY_TAG = String("humidity"); 
  
};
