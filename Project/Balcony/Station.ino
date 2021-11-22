#include "src/NRF24Transmitter.h"
#include "src/AHT10Sensor.h"

#include <printf.h>


void setup() {
	Serial.begin(9600);
	printf_begin();
	Radio::NRF24.Init(Radio::PIPE_0_ADDR);
	Sensors::AHT10.Init();
}

void loop() {
	delay(2000);
	Radio::data[0] = 0;
	Radio::data[1] = 2;
	Radio::data[2] = Sensors::AHT10.GetTemperature();
	Radio::data[3] = Sensors::AHT10.GetHumidity();
	Radio::data[4] = 0;
	Radio::NRF24.TransmitData();
}