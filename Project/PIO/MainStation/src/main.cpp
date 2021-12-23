// MAIN RECEIVER
#include "NRFDataParser.h"
#include "NRF24Receiver.h"
#include <printf.h>

const size_t GLOBAL_DELAY_TIME = 3000;
Radio::BalconyNRFDataParser nrfBalconyDataParser;

void setup() {
    Serial.begin(9600);
    printf_begin();
    Radio::NRF24.Init();
    Radio::NRF24.AddPipeForListening(Radio::PIPE_0_ADDR);
    Radio::NRF24.StartListeningAll();
}

void loop() {
    Radio::receivingAttempts++;
    if (Radio::NRF24.IsAvailable())
    {
        Radio::NRF24.ReceiveData();
        if (Radio::DataIsValid()) {           
            Serial.println("[I] Data received at attempt: " + String(Radio::receivingAttempts));
            Serial.println("[I] Sending new JSON");
            nrfBalconyDataParser.SendNewJsonToESP();
            nrfBalconyDataParser.ClearReceivedData();
            Radio::receivingAttempts = 0;
            delay(GLOBAL_DELAY_TIME);
        }
    }
      
   if (Radio::receivingAttempts > Radio::MAX_RECEIVING_ATTEMPTS) {
        Serial.println("[E] Data did not received, send previous values to JSON");
        nrfBalconyDataParser.SendOldJsonToESP();
        Radio::receivingAttempts = 0;
    }
}
