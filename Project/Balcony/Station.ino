#include <ArduinoJson.hpp>
#include "src/BMP280Sensor.h"
#include "src/AHT10Sensor.h"
#include <ArduinoJson.h>
#include <SPI.h>                                               // Подключаем библиотеку для работы с шиной SPI.
#include <nRF24L01.h>                                          // Подключаем файл настроек из библиотеки RF24.
#include <RF24.h>                                              // Подключаем библиотеку для работы с nRF24L01+.
                  

RF24 radio(9, 10);

StaticJsonDocument<200> jsonDoc;

void setup() {
    delay(500);
    Serial.begin(9600);
    // UART
   // Serial1.begin(9600);
    //
    //Sensors::BMP280.Init();
    Sensors::AHT10.Init();         

    radio.begin();           
    Serial.print("Connected ");  
    if (radio.isPVariant() && radio.isChipConnected() && radio.isValid()) 
    {
        Serial.println("nRF24L01");    
    } 
    else 
    { 
        Serial.print("unknown module"); 
    } 

 }

void loop() {    
    Serial.println(Sensors::AHT10.GetTemperature());
    delay(1000);
}
