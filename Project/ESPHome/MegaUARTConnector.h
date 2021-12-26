#include "esphome.h"
#include <ArduinoJson.h>

class MegaUARTConnector : public PollingComponent, public UARTDevice, public Sensor {
public:
   
    MegaUARTConnector(UARTComponent *parent) 
        : PollingComponent(GLOBAL_DELAY), 
          UARTDevice(parent)
    {
        aht10TempBalcony = new Sensor();
        aht10HumidityBalcony = new Sensor();
        ds18b20TempBalconyStreet = new Sensor();
        
        aht10TempHall = new Sensor();
        aht10HumidityHall = new Sensor();
        _aht10HallInited = false;
    }

    void setup() override {
        if (_aht10HallMain.begin()) {
            _aht10HallInited = true;
            delay(AHT_INIT_DELAY);
            _aht_temp = _aht10HallMain.getTemperatureSensor();
            _aht_humidity = _aht10HallMain.getHumiditySensor();
        } 
        _ahtHallTimer = millis();
    }
  
    void update() override {
        while (available()) {
            char ch = read();
            while (ch != '\n') {
                _dataBuf += ch;
                ch = read();
            }
            DeserializationError error = deserializeJson(_doc, _dataBuf);
            if (!error) {
                //ESP_LOGD("UART", "json is valid");
                _temp = _doc[TEMP_TAG].as<float>();
                _humidity = _doc[HUMIDITY_TAG].as<float>();
                _tempStreet = _doc[TEMP_STREET_TAG].as<float>();
                _dataBuf = "";
            } else {
                _dataBuf = "";
            }
        }
        
        aht10TempBalcony->publish_state(_temp);
        aht10HumidityBalcony->publish_state(_humidity);
        ds18b20TempBalconyStreet->publish_state(_tempStreet);
        
       if (millis() - _ahtHallTimer >= AHT_DELAY) {   
            _ahtHallTimer = millis();              
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
        delay(GLOBAL_DELAY);
    }
 
public:
    Sensor* aht10TempBalcony;
    Sensor* aht10HumidityBalcony;
    Sensor* ds18b20TempBalconyStreet;
    
    Sensor* aht10TempHall;
    Sensor* aht10HumidityHall;

private:

    StaticJsonDocument<200> _doc;
    Adafruit_AHT10 _aht10HallMain;
    Adafruit_Sensor* _aht_humidity;
    Adafruit_Sensor* _aht_temp;
    bool _aht10HallInited = false;

    
    String _dataBuf;
    float _temp;
    float _tempStreet;
    float _humidity;
    int _ahtHallTimer;
    const int RADIO_DELAY = 10;

    const int AHT_INIT_DELAY = 200;
    const int AHT_DELAY = 5000;
    const int GLOBAL_DELAY = 20;
    const String TEMP_TAG = String("temperature");
    const String PRESSURE_TAG = String("pressure");
    const String HUMIDITY_TAG = String("humidity"); 
    const String TEMP_STREET_TAG = String("temperature.street");
  
};
