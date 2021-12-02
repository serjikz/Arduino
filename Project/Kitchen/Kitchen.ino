// MAIN RECEIVER
#include <Wire.h> 
#include "src/NRFDataParser.h"
#include "src/NRF24Receiver.h"
#include <printf.h>
#include <LiquidCrystal_I2C.h>


const size_t GLOBAL_DELAY_TIME = 10;
Radio::BalconyNRFDataParser nrfBalconyDataParser;
LiquidCrystal_I2C lcd(0x27, 16, 2);  

void setup() {
    Serial.begin(9600);
    printf_begin();
    lcd.init();        
    lcd.backlight();
    lcd.setCursor(5, 0);
    lcd.print("Loading...");
    Radio::NRF24.Init();
    Radio::NRF24.AddPipeForListening(Radio::PIPE_0_ADDR);
    Radio::NRF24.StartListeningAll();    
}

void loop() {
    // символ, строка
    lcd.setCursor(0, 0);
    lcd.print("T balc " + String(nrfBalconyDataParser.GetTemperature()) + " *C");
    lcd.setCursor(0, 1);
    lcd.print("T str  " + String(nrfBalconyDataParser.GetTemperatureOnStreet()) + " *C");

    Radio::receivingAttempts++;
    if (Radio::NRF24.IsAvailable())
    {
        Radio::NRF24.ReceiveData();
        if (Radio::DataIsValid()) {           
            Serial.println("Data received at attempt: " + String(Radio::receivingAttempts));           
            nrfBalconyDataParser.SaveReceivedData();
            nrfBalconyDataParser.PrintDebugInfo();           
            Radio::receivingAttempts = 0;
            delay(GLOBAL_DELAY_TIME);
        }
    }
      
   if (Radio::receivingAttempts > Radio::MAX_RECEIVING_ATTEMPTS) {
        Serial.println("Data did not received, " + String(Radio::MAX_RECEIVING_ATTEMPTS) + " attempts failed");
        Radio::receivingAttempts = 0;
    }
}
