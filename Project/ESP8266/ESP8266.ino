#include <Wire.h>
#include <SPI.h>

/*  ¬ библиотеке Wire используетс€ 7 - ми битна€ адресаци€ устройств.
    јдреса с 0 до 7 не используютс€, так как зарезервированы, поэтому 
    первым адресом, который может быть использован, €вл€етс€ 8. 
    ћы будем использовать 2 адреса дл€ подключени€ устройств : 8 и 13. 
*/

void setup() {
    Serial.begin(9600); /* открываем серийный порт дл€ дебаггинга */
    Wire.begin(D1, D2); /* задаем i2c мост через контакты SDA=D1 и SCL=D2 на NodeMCU */
}

void loop() {
    Serial.println("Trying connect to Mega");
    Wire.beginTransmission(8); /* Ќачинаем передачу на адресе 8 */
    Wire.write("Hello Arduino");  /* ќтправл€ем "hello Arduino" */
    Wire.endTransmission();    /* прекращаем передачу */

    Wire.requestFrom(8, 5); /* запрашиваем и считываем данные с 8 адреса 13 байт slave устройства */
    while (Wire.available()) {
        char c = Wire.read();
        Serial.print(c);
    }
    Serial.println();
    delay(1000);
}
