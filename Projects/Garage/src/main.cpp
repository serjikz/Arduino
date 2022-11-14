#include "AHT10Sensor.h"
#include "LCD1602Display.h"
#include "RGYColorsIniter.h"
#include <printf.h>

const int GLOBAL_DELAY_TIME = 3000;
unsigned long long time = 0;

RGYColorsIniter rgyIniter(PIND3, PIND2, PIND4);

//
Adafruit_AHT10 AHT10Internal;
Adafruit_AHT10 AHT10External;
AHT10Sensor aht10Internal(&AHT10Internal, String("AHT10Internal"));
AHT10Sensor aht10External(&AHT10External, String("AHT10External"));
//

void TCA9548A(uint8_t bus){
    Wire.beginTransmission(0x70);  // TCA9548A address
    Wire.write(1 << bus);          // send byte to select bus
    Wire.endTransmission();
}

void setup()
{    
    Serial.begin(9600);
    Wire.begin();
    printf_begin();
    rgyIniter.Start();
    //Display::LCD1602.Init();
    TCA9548A(0);
    aht10Internal.Init();
    TCA9548A(1);
    aht10External.Init();
}

void loop()
{
    rgyIniter.Update();
    if (millis() - time >= GLOBAL_DELAY_TIME) {
        time = millis();
        TCA9548A(0);
        String internalT = "T int=" + String(aht10Internal.GetTemperature());        
        TCA9548A(1);
        String externalT = "T ext=" + String(aht10External.GetTemperature());   
        // Display::LCD1602.Clear();  
        // Display::LCD1602.Print(0, 0, internalT);
        // Display::LCD1602.Print(0, 1, externalT);  
        Serial.println(internalT);    
        Serial.println(externalT);    
        rgyIniter.BlinkGreenLight();
    }
}
