#include <Arduino.h>
#include <LCD1602Display.h>
#include <RGYColorsIniter.h>
#include <printf.h>

const int GLOBAL_DELAY_TIME = 3000;
unsigned long long time = 0;

RGYColorsIniter rgyIniter(PIND3, PIND2, PIND4);

void setup()
{    
    Serial.begin(9600);
    Wire.begin();
    printf_begin();
    Display::LCD1602.Clear();
    rgyIniter.Start();
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
