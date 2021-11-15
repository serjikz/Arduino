#include "src/BMP280Sensor.h"
#include "src/AHT10Sensor.h"

void setup() {
    Serial.begin(9600);
    // UART
    Serial1.begin(9600);
    //
    Sensors::BMP280.Init();
    Sensors::AHT10.Init();
 }


void loop() {
    //Serial1.println(Sensors::BMP280.GetID());
    Serial1.write(Sensors::BMP280.GetTemperature().c_str());
    Serial1.write("/");
    Serial1.write(Sensors::BMP280.GetPressure().c_str());
    Serial1.write("#");
    //Serial.println(Sensors::AHT10.GetID());
    //Serial.println(Sensors::AHT10.GetTemperature());
    //Serial.println(Sensors::AHT10.GetHumidity());
    //Serial.println("Sending data to ESP");
    //Serial1.write("123456#");
    delay(100);
}
