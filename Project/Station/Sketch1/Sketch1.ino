#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp; // I2C
//Adafruit_BMP280 bme(BMP_CS); // hardware SPI
//Adafruit_BMP280 bme(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);

void setup() {
    Serial.begin(9600);
    Serial.println(F("BMP280 test"));

    if (!bmp.begin()) {
        Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    }
    else {
        bmp.setSampling(Adafruit_BMP280::MODE_NORMAL, // Режим работы
            Adafruit_BMP280::SAMPLING_X2, // Точность изм. температуры
            Adafruit_BMP280::SAMPLING_X16, // Точность изм. давления
            Adafruit_BMP280::FILTER_X16, // Уровень фильтрации
            Adafruit_BMP280::STANDBY_MS_4000); // Период просыпания, мСек
    }
}

void loop() {
    Serial.print(F("Temperature = "));
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");

    Serial.print(F("Pressure = "));
    Serial.print(0.00750062 * bmp.readPressure());
    Serial.println(" Pa");

    Serial.print(F("Approx altitude = "));
    Serial.print(bmp.readAltitude(1013.25)); // this should be adjusted to your local forcase
    Serial.println(" m");

    Serial.println();
    delay(2000);
}