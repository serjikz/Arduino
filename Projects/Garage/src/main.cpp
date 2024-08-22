#include <Arduino.h>
#include <LCD1602Display.h>
#include <RGYColorsIniter.h>
#include <ENS160AHT2X.h>
#include <printf.h>

const int GLOBAL_DELAY_TIME = 3000;
unsigned long long time = 0;

RGYDiodsInformer rgyDiodsInformer(PIND2, PIND3, PIND4);
ENS160AHT2X ensAht2x(String("ENS160AHT2X"));

void setup()
{    
    Serial.begin(9600);
    Wire.begin();
    printf_begin();
    
    rgyDiodsInformer.Start();
    Display::LCD1602.Init();
    ensAht2x.Init();   
}

void DisplaySensorData()
{
    Serial.println(ensAht2x.GetTemperature());
    Serial.println(ensAht2x.GetHumidity());
    rgyDiodsInformer.BlinkGreenLight();

    String vals[2];
    vals[0] = ensAht2x.GetTemperature();
    vals[1] = ensAht2x.GetHumidity();
    Display::LCD1602.Clear();
    Display::LCD1602.UpdateValues(vals);
}

void ShowErrorState()
{
    rgyDiodsInformer.BlinkRedLight();
    if (Display::LCD1602.IsInited()) {
        Display::LCD1602.Clear();
        Display::LCD1602.ShowErrMsg();
    }
}

void loop()
{
    rgyDiodsInformer.Update();
    Display::LCD1602.Update();
    Display::LCD1602.ShowStoragedData();
    
    if (millis() - time >= GLOBAL_DELAY_TIME) {
        time = millis();        
        if (ensAht2x.IsInited()) {
           DisplaySensorData();
        } else {
            ShowErrorState();
        }
    }
}
