#include <Arduino.h>
#include <LCD1602Display.h>
#include <RGYColorsIniter.h>
#include <ENS160AHT2X.h>
#include <printf.h>

const int GLOBAL_DELAY_TIME = 3000;
unsigned long long time = 0;

RGYColorsIniter rgyIniter(PIND2, PIND3, PIND4);
ENS160AHT2X ensAht2x(String("ENS160AHT2X"));

void setup()
{    
    Serial.begin(9600);
    Wire.begin();
    printf_begin();
    
    rgyIniter.Start();
    Display::LCD1602.Init();
    ensAht2x.Init();
    
}

void loop()
{
    rgyIniter.Update();
    if (millis() - time >= GLOBAL_DELAY_TIME) {
        time = millis();
        // Display::LCD1602.Clear();  
        // Display::LCD1602.Print(0, 0, internalT);
        // Display::LCD1602.Print(0, 1, externalT);   
        rgyIniter.BlinkGreenLight();
    }
}
