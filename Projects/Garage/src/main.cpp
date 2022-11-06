// GARAGE TRANSMITTER (UNO)
#include <LoRa.h>
#include "LoRaSettings.h"
#include "JSONSender.h"
#include "AHT10Sensor.h"
#include "LCD1602Display.h"
#include "RGYColorsIniter.h"
#include <printf.h>

const int GLOBAL_DELAY_TIME = 3000;

void setup()
{
    RGYColorsIniter rgyIniter(PIND3, PIND2, PIND4);
    Display::LCD1602.Init();
    Sensors::AHT10.Init();
    Serial.begin(9600);
    printf_begin();
}

void loop()
{
    digitalWrite(PIND2, HIGH);
    digitalWrite(PIND3, HIGH);
    digitalWrite(PIND4, HIGH);
    delay(GLOBAL_DELAY_TIME);
    digitalWrite(PIND2, LOW); 
    digitalWrite(PIND3, LOW);
    digitalWrite(PIND4, LOW);

    Display::LCD1602.Print(0, 0, String(Sensors::AHT10.GetTemperature()));
    Serial.println("T= " + String(Sensors::AHT10.GetTemperature()));
   
    delay(GLOBAL_DELAY_TIME);
}
