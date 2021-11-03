#include "src/BMP280Sensor.h"
#include "src/AHT10Sensor.h"
#include <Wire.h>

void setup() {
    Serial.begin(9600);
    Sensors::BMP280.Init();
    Sensors::AHT10.Init();
    Wire.begin(8);                /* ������ �� ���� i2c 8 ����� */
    Wire.onReceive(receiveEvent); /* ������������ ���������� ������� */
    Wire.onRequest(requestEvent); /* ������������ ����������� ������� */
}

// ������� ��� ���������� ����� ����������� ������ �� ������� �� ����
void receiveEvent(int howMany) {
    while (0 < Wire.available()) {
        char c = Wire.read();      /* �������� ���� ��� ������*/
        Serial.print(c);           /* ������� ������ � �������� ���� */
    }
    Serial.println();             /* ��������� �� ����� ������ */
}

// ������� ��� ���������� ����� ������������ ������ �� ������� �� ����
void requestEvent() {
    //Wire.write("Hello NodeMCU");
    String msg = Sensors::BMP280.GetTemperature();
    Wire.write(msg.c_str());
}

void loop() {
    Serial.println(Sensors::BMP280.GetID());
    Serial.println(Sensors::BMP280.GetTemperature());
    Serial.println(Sensors::BMP280.GetPressure());
    Serial.println(Sensors::AHT10.GetID());
    Serial.println(Sensors::AHT10.GetTemperature());
    Serial.println(Sensors::AHT10.GetHumidity());
    delay(5000);
}
