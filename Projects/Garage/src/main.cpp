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

void setup()
{    
    rgyIniter.Start();
    Display::LCD1602.Init();
    Sensors::AHT10.Init();
    Serial.begin(9600);
    printf_begin();
}

void loop()
{
    rgyIniter.Update();
    if (millis() - time >= GLOBAL_DELAY_TIME) {
        time = millis();
        Display::LCD1602.Print(0, 0, String(Sensors::AHT10.GetTemperature()));
        Serial.println("T= " + String(Sensors::AHT10.GetTemperature()));    
        if (rgyIniter.IsStartEffectCompleted())
        {
            rgyIniter.BlinkGreenLight();    
        }
    }
}
