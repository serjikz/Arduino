// Balcony transmitter

#include "NRF24Transmitter.h"
#include "AHT10Sensor.h"
#include "DS18B20Sensor.h"

#include <printf.h>
static const size_t GLOBAL_DELAY_TIME = 5000;

void setup()
{
    Serial.begin(9600);
    printf_begin();
    Radio::NRF24.Init(Radio::PIPE_0_ADDR);
    Sensors::AHT10.Init();
    Sensors::DS18B20.Init();
}

void loop()
{
    delay(GLOBAL_DELAY_TIME);
    Radio::data[0] = Radio::HASH;
    Radio::data[1] = Radio::ModulePosition::Balcony;
    Radio::data[2] = Radio::SensorsCount::Three;
    Radio::data[3] = Sensors::AHT10.GetTemperature();
    Radio::data[4] = Sensors::AHT10.GetHumidity();
    Radio::data[5] = Sensors::DS18B20.GetTemperature();
    Radio::NRF24.TransmitData();
}