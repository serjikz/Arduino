// KITCHEN RECEIVER
#include <Wire.h> 
#include "NRFDataParser.h"
#include "NRF24Receiver.h"
#include "LCD1602Display.h"
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
    Radio::receivingPauseTimer = millis();
}

void loop() {
    Display::LCD1602.Update();
    Display::LCD1602.ShowStoragedData();
    if (Radio::NRF24.IsAvailable() && Radio::ReadyToGetNextData())
    {
        Radio::receivingAttempts++;
        Radio::NRF24.ReceiveData();
        if (Radio::DataIsValid()) {
            Serial.println("[I] Data received at attempt: " + String(Radio::receivingAttempts));           
            nrfBalconyDataParser.SaveReceivedData();
            nrfBalconyDataParser.PrintDebugInfo();           
            float vals[3];
            vals[0] = nrfBalconyDataParser.GetTemperature();
            vals[1] = nrfBalconyDataParser.GetTemperatureOnStreet();
            vals[2] = nrfBalconyDataParser.GetHumidity();
            Display::LCD1602.UpdateValues(vals);
            Radio::ResetDataReceivingProcess();
        }
    }
      
   if (Radio::receivingAttempts >= Radio::MAX_RECEIVING_ATTEMPTS) {
        Serial.println("[E] Data did not received, " + String(Radio::receivingAttempts) + " attempts failed");
        Radio::receivingAttempts = 0;
   }
       
}
