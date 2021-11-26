#include "esphome.h"
#include <Adafruit_AHT10.h>

class AHT10Sensor : public PollingComponent {
public:

    AHT10Sensor() 
        : PollingComponent(GLOBAL_DELAY) 
    {
        aht10TempSensor = new Sensor();
        aht10HumiditySensor = new Sensor();
    }

    void setup() override {
        if (_aht10Main.begin()) {
            delay(AHT_INIT_DELAY);
            _aht_temp = _aht10Main.getTemperatureSensor();
            _aht_humidity = _aht10Main.getHumiditySensor();
        }
    }
  
    void update() override {
        sensors_event_t temp;
        _aht_temp->getEvent(&temp);
        aht10TempSensor->publish_state(temp.temperature);
        
        sensors_event_t humidity;
        _aht_humidity->getEvent(&humidity);
        aht10HumiditySensor->publish_state(humidity.relative_humidity);
    }

    Sensor* aht10TempSensor;
    Sensor* aht10HumiditySensor;
    
private:
    Adafruit_AHT10 _aht10Main;
    Adafruit_Sensor* _aht_humidity;
    Adafruit_Sensor* _aht_temp;
    const int GLOBAL_DELAY = 2000;
    const int AHT_INIT_DELAY = 200;
    const String TEMP_TAG = String("temperature");
    const String HUMIDITY_TAG = String("humidity"); 

};
