// GARAGE TRANSMITTER (UNO)
#include <LoRa.h>
#include "LoRaSettings.h"
#include "JSONSender.h"
#include "AHT10Sensor.h"
#include "LCD1602Display.h"
#include "RGYColorsIniter.h"
#include <printf.h>

const int GLOBAL_DELAY_TIME = 3000;
unsigned long long time = 0;

RGYColorsIniter rgyIniter(PIND3, PIND2, PIND4);

Adafruit_AHT10 AHT10Internal;
Adafruit_AHT10 AHT10External;
AHT10Sensor aht10Internal(&AHT10Internal, String("AHT10Internal"));
AHT10Sensor aht10External(&AHT10External, String("AHT10External"));

void setup()
{    
    rgyIniter.Start();
    Display::LCD1602.Init();
    aht10Internal.Init();
    aht10External.Init();
    Serial.begin(9600);
    printf_begin();
}

void loop()
{
    rgyIniter.Update();
    if (millis() - time >= GLOBAL_DELAY_TIME) {
        time = millis();
        String internalT = "T int=" + String(aht10Internal.GetTemperature());
        String externalT = "T ext=" + String(aht10External.GetTemperature());   
        Display::LCD1602.Clear();  
        Display::LCD1602.Print(0, 0, internalT);
        Display::LCD1602.Print(0, 1, externalT);  
        Serial.println(internalT);    
        Serial.println(externalT);    
        rgyIniter.BlinkGreenLight();
    }
}
