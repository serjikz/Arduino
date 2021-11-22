// MAIN RECEIVER

#include <ArduinoJson.hpp>
#include "src/NRF24Receiver.h"
#include <printf.h>
#include <ArduinoJson.h>    

static const size_t GLOBAL_DELAY_TIME = 2000;
StaticJsonDocument<200> jsonDoc;

void setup() {
    Serial.begin(9600);
    printf_begin();
    Radio::NRF24.Init();
    Radio::NRF24.AddPipeForListening(Radio::PIPE_0_ADDR);
    Radio::NRF24.StartListeningAll();
}

void loop() {
    if (Radio::NRF24.IsAvailable())
    {
        Radio::NRF24.ReceiveData();
        Serial.println("From Balcony[0]: " + String(Radio::data[0]));
        Serial.println("From Balcony[1]: " + String(Radio::data[1]));
        Serial.println("From Balcony[2]: " + String(Radio::data[2]));
        Serial.println("From Balcony[3]: " + String(Radio::data[3]));
        Serial.println("From Balcony[4]: " + String(Radio::data[4]));
    }
    jsonDoc.clear();
    jsonDoc["temp"] = Radio::data[2];
    jsonDoc["pressure"] = -1;
    jsonDoc["humidity"] = Radio::data[3];
    serializeJson(jsonDoc, Serial);
    Serial.write('\n');
    delay(GLOBAL_DELAY_TIME);
}