// GARAGE TRANSMITTER (MEGA)
#include <LoRa.h>
#include "LoRaSettings.h"

void setup()
{
    Serial.begin(9600);
    while (!Serial);
    //LoRa.setPins(LoRaSettings::CS_PIN, LoRaSettings::RST_PIN, LoRaSettings::IRQ_PIN);
    if (!LoRa.begin(LoRaSettings::FREQUENCY))
    {
        Serial.println("LoRa init failed. Check your connections.");
    }
    else
    {
        Serial.println("LoRa inited.");
    }
}

void loop()
{
}
