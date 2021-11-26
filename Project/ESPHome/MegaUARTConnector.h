#include "esphome.h"
#include <ArduinoJson.h>
#include <Adafruit_AHT10.h>
#include <Wire.h>
#include <SPI.h>

class MegaUARTConnector : public PollingComponent, public UARTDevice, public Sensor {
public:
   
    MegaUARTConnector(UARTComponent *parent) 
        : PollingComponent(GLOBAL_DELAY), 
          UARTDevice(parent)
    {
        aht10TempBalcony = new Sensor();
        aht10HumidityBalcony = new Sensor();
        
        aht10TempHall = new Sensor();
        aht10HumidityHall = new Sensor();
        _aht10HallInited = false;
    }

    void setup() override {
        if (_aht10Main.begin()) {
            _aht10HallInited = true;
            delay(AHT_INIT_DELAY);
            _aht_temp = _aht10Main.getTemperatureSensor();
            _aht_humidity = _aht10Main.getHumiditySensor();
        } 
    }
  
    void update() override {
        while (available()) {
            char ch = read();
            while (ch != '\n') {
                _dataBuf += ch;
                ch = read();
            }
            DynamicJsonBuffer doc(JSON_DOC_SIZE);
            JsonObject& root = doc.parseObject(_dataBuf);
            if (root.size() > 0) {
                ESP_LOGD("UART", "json is valid");
                _temp = root[TEMP_TAG].as<float>();
                _humidity = root[HUMIDITY_TAG].as<float>();
                _dataBuf = "";
            } else {
                _dataBuf = "";
            }
        }
        
        aht10TempBalcony->publish_state(_temp);
        aht10HumidityBalcony->publish_state(_humidity);
        
        if (_aht10HallInited) {
            sensors_event_t temp;
            _aht_temp->getEvent(&temp);
            aht10TempHall->publish_state(temp.temperature);
        
            sensors_event_t humidity;
            _aht_humidity->getEvent(&humidity);
            aht10HumidityHall->publish_state(humidity.relative_humidity);
        } else {
            aht10TempHall->publish_state(-1);
            aht10HumidityHall->publish_state(-1);
        }
    }
 
public:
    Sensor* aht10TempBalcony;
    Sensor* aht10HumidityBalcony;
    
    Sensor* aht10TempHall;
    Sensor* aht10HumidityHall;

private:
    Adafruit_AHT10 _aht10Main;
    Adafruit_Sensor* _aht_humidity;
    Adafruit_Sensor* _aht_temp;
    bool _aht10HallInited = false;
    
    String _dataBuf;
    float _temp;
    float _humidity;
    const int GLOBAL_DELAY = 100;
    const int JSON_DOC_SIZE = 200;
    const int AHT_INIT_DELAY = 200;
    const String TEMP_TAG = String("temperature");
    const String PRESSURE_TAG = String("pressure");
    const String HUMIDITY_TAG = String("humidity"); 
  
};
