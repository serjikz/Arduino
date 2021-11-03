#include <Wire.h>
#include <SPI.h>

/*  � ���������� Wire ������������ 7 - �� ������ ��������� ���������.
    ������ � 0 �� 7 �� ������������, ��� ��� ���������������, ������� 
    ������ �������, ������� ����� ���� �����������, �������� 8. 
    �� ����� ������������ 2 ������ ��� ����������� ��������� : 8 � 13. 
*/

void setup() {
    Serial.begin(9600); /* ��������� �������� ���� ��� ���������� */
    Wire.begin(D1, D2); /* ������ i2c ���� ����� �������� SDA=D1 � SCL=D2 �� NodeMCU */
}

void loop() {
    Serial.println("Trying connect to Mega");
    Wire.beginTransmission(8); /* �������� �������� �� ������ 8 */
    Wire.write("Hello Arduino");  /* ���������� "hello Arduino" */
    Wire.endTransmission();    /* ���������� �������� */

    Wire.requestFrom(8, 5); /* ����������� � ��������� ������ � 8 ������ 13 ���� slave ���������� */
    while (Wire.available()) {
        char c = Wire.read();
        Serial.print(c);
    }
    Serial.println();
    delay(1000);
}
