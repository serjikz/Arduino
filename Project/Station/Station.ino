// MAIN RECEIVER
#include "src/NRFDataParser.h"
#include "src/NRF24Receiver.h"
#include <printf.h>

const size_t GLOBAL_DELAY_TIME = 2000;
Radio::BalconyNRFDataParser nrfBalconyDataParser;

void setup() {
    Serial.begin(9600);
    printf_begin();
    Radio::NRF24.Init();
    Radio::NRF24.AddPipeForListening(Radio::PIPE_0_ADDR);
    Radio::NRF24.StartListeningAll();
}

void loop() {
    bool received = false;
    Radio::receivingAttempts++;
    if (Radio::NRF24.IsAvailable())
    {
        Radio::NRF24.ReceiveData();
        if (Radio::DataIsValid()) {           
            Serial.println("From Balcony[0]: " + String(Radio::receivedData[0]));
            Serial.println("From Balcony[1]: " + String(Radio::receivedData[1]));
            Serial.println("From Balcony[2]: " + String(Radio::receivedData[2]));
            Serial.println("From Balcony[3]: " + String(Radio::receivedData[3]));
            Serial.println("From Balcony[4]: " + String(Radio::receivedData[4]));
            received = true;
        }
    }

    if (Radio::receivingAttempts > Radio::MAX_RECEIVING_ATTEMPTS) {
        Serial.println("Data did not received, send previous values to JSON");
        nrfBalconyDataParser.SendOldJsonToESP();
        Radio::receivingAttempts = 0;
    }
    else if (received) {
        Serial.println("Data received at attempt: " + String(Radio::receivingAttempts));
        Serial.println("Sending new JSON");
        nrfBalconyDataParser.SendNewJsonToESP();
        nrfBalconyDataParser.ClearReceivedData();
        Radio::receivingAttempts = 0;
        delay(GLOBAL_DELAY_TIME);        
    }       
}
