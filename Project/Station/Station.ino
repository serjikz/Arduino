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
    bool received = false;
    Radio::receivngAttempts++;
    if (Radio::NRF24.IsAvailable())
    {
        Radio::NRF24.ReceiveData();
        if (abs(Radio::receivedData[0] - Radio::HASH) < Radio::EPS) {
            Serial.println("Data received at attempt: " + String(Radio::receivngAttempts));
            Serial.println("From Balcony[0]: " + String(Radio::receivedData[0]));
            Serial.println("From Balcony[1]: " + String(Radio::receivedData[1]));
            Serial.println("From Balcony[2]: " + String(Radio::receivedData[2]));
            Serial.println("From Balcony[3]: " + String(Radio::receivedData[3]));
            Serial.println("From Balcony[4]: " + String(Radio::receivedData[4]));
            //Serial.println("From Balcony[5]: " + String(Radio::receivedData[5]));
            // Serial.println("From Balcony[6]: " + String(Radio::receivedData[6]));
            Radio::CopyDataToJson();
            Radio::ClearReceivedData();
            received = true;
            Radio::receivngAttempts = 0;
        }
    }

    if (Radio::receivngAttempts > Radio::MAX_RECEIVING_ATTEMPTS) {
        Serial.println("Data did not received, send previous values to JSON");
    }
    else if (received) {
        Serial.println("Data has been received, send new JSON");
    }

    if (Radio::receivngAttempts > Radio::MAX_RECEIVING_ATTEMPTS || received) {
        jsonDoc.clear();
        jsonDoc["temp"] = Radio::jsonData[3];
        jsonDoc["pressure"] = -1;
        jsonDoc["humidity"] = Radio::jsonData[4];
        serializeJson(jsonDoc, Serial);
        Serial.write('\n');
        Radio::receivngAttempts = 0;
    } 

    if (received) {
        delay(GLOBAL_DELAY_TIME);
    }
}