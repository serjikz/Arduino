// KITCHEN RECEIVER
#include <Wire.h> 
#include "src/NRFDataParser.h"
#include "src/NRF24Receiver.h"
#include "src/LCD1602Display.h"
#include <printf.h>

const size_t GLOBAL_DELAY_TIME = 2000;
Radio::BalconyNRFDataParser nrfBalconyDataParser;

void setup() {
    Serial.begin(9600);
    printf_begin();
    Display::LCD1602.Init();
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
            nrfBalconyDataParser.SaveReceivedData();
            nrfBalconyDataParser.PrintDebugInfo();           
            Radio::receivingAttempts = 0;
            Display::LCD1602.Print(0, 0, nrfBalconyDataParser.GetDisplayTempBalcony());
            Display::LCD1602.Print(0, 1, nrfBalconyDataParser.GetDisplayTempStreet());
            delay(GLOBAL_DELAY_TIME);
        }
    }
      
   if (Radio::receivingAttempts >= Radio::MAX_RECEIVING_ATTEMPTS) {
        Serial.println("[E] Data did not received, " + String(Radio::receivingAttempts) + " attempts failed");
        Radio::receivingAttempts = 0;
    }
}
