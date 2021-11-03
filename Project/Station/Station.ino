#include "src/BMP280Sensor.h"
#include "src/AHT10Sensor.h"

void setup() {
    Serial.begin(9600);
    Sensors::BMP280.Init();
    Sensors::AHT10.Init();
}

void loop() {
    Serial.println(Sensors::BMP280.GetID());
    Serial.println(Sensors::BMP280.GetTemperature());
    Serial.println(Sensors::BMP280.GetPressure());
    Serial.println(Sensors::AHT10.GetID());
    Serial.println(Sensors::AHT10.GetTemperature());
    Serial.println(Sensors::AHT10.GetHumidity());
    delay(1000);
}
