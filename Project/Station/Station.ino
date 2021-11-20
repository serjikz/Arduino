#include <ArduinoJson.hpp>
#include "src/BMP280Sensor.h"
#include "src/AHT10Sensor.h"
#include <ArduinoJson.h>

StaticJsonDocument<200> jsonDoc;

void setup() {
    Serial.begin(9600);
    // UART
    Serial1.begin(9600);
    //
    Sensors::BMP280.Init();
    Sensors::AHT10.Init();         
 }

void loop() {
    jsonDoc.clear();
    jsonDoc["temp"] = Sensors::BMP280.GetTemperature();
    jsonDoc["pressure"] = Sensors::BMP280.GetPressure();
    jsonDoc["humidity"] = Sensors::AHT10.GetHumidity();
    serializeJson(jsonDoc, Serial1);
    Serial1.write('\n');
    delay(100);
}
