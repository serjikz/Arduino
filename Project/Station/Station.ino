#include "src/BMP280Sensor.h"

void setup() {
    Serial.begin(9600);
    Serial.println(F("BMP280 test"));
    Sensors::BMP280.Init();
}

void loop() {
    Serial.println(Sensors::BMP280.GetTemperature());
    Serial.println(Sensors::BMP280.GetPressure());
    delay(4000);
}
