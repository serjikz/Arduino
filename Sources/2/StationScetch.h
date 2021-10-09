/* СОЕДИНЕНИЯ
Подключение датчика температуры-влажности DHT11 ---
- OUT  > цифровой пин 9
- "+"  > +3V (+5V)
- "-"  > GND
Подключение датчика давления-температуры BMP085 ---
- VCC  > 3.3V; (поддерживается 1.8 - 3.6 В)
- GND  > GND
- SDA  > аналоговый пин A4 
- SCL  > аналоговый пин A5 
Детектор газа --------------------------------------
- A0
- VCC
- GND
ЖКИ ------------------------------------------------
- LCD RS 	 > к цифровым выводам 12 - 8
- LCD Enable > к выводам 11 - 9
- LCD D4 	 > к выводам 5 - 4
- LCD D5 	 > к выводам 4 - 5
- LCD D6 	 > к выводам 3 - 6
- LCD D7 	 > к выводам 2 - 7
- LCD R/W 	 > GND
- 10K резистор между +5V и землёй
- сброс 	 > к LCD VO (пин 3)
*/
// Подключение библиотек:
#include <dht11.h> // датчика температуры-влажности 
#include <LiquidCrystal.h> // LCD 
#include <Wire.h> // датчика BMP085 
#include <MQ135.h> // газовый детектор MQ135

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); /* соединение LCD-шилда
\x## - для вывода кириллических символов, где ## - номера символов в таблице */
dht11 sensorTempHumid; // датчик температуры и влажности
MQ135 gasSensor = MQ135(A0); // датчик газа
#define RZERO 76.63
float rzero; // показания с датчика газа
float ppm;

int del = 5000; // задержка, мс

/* Oversampling Setting (OSS) - 
настройки разрешения для датчика давления BMP085, 
см. https://www.sparkfun.com/tutorials/253 
OSS может быть 0, 1, 2 или 3:
0 - ультранизкое потребление, низкое разрешение;
1 - стандартное потребление;
2 - высокое разрешение;
3 - ультравысокое разрешение и максимальное потребление. */
const unsigned char OSS = 0; 

// Калибровочные переменные для BMP085:
int ac1;
int ac2;
int ac3;
unsigned int ac4;
unsigned int ac5;
unsigned int ac6;
int b1;
int b2;
int mb;
int mc;
int md;
long b5;

float temperature; // температура
long pressure; // давление

// Для прогноза:
const float p0 = 101325; // давление на уровне моря, Па.
const float currentAltitude = 179.5; // высота погодной станции над уровнем моря, м;
const float ePressure = p0 * pow((1 - currentAltitude/44330), 5.255);  // нормальное давление на данной высоте, Па.
float weatherDiff;

#define DHT11PIN 9 // пин 9 датчика DHT11.
#define BMP085_ADDRESS 0x77  /* I2C адрес датчика BMP085; 
см. https://www.sparkfun.com/tutorials/253  */

void setup() {
  lcd.begin(16, 2); // инициализация ЖК
  Wire.begin(); // инициализация I2C
  bmp085Calibration(); // калибровка датчика BMP085
}

void loop() {
  // Проверка датчика давления-влажности:
  int chk = sensorTempHumid.read(DHT11PIN);
  
  switch (chk) {
    case DHTLIB_OK: 
                lcd.clear();
                break;
    case DHTLIB_ERROR_CHECKSUM: 
                lcd.clear();
                lcd.print("Checksum error");
                delay(del); 
                return;
    case DHTLIB_ERROR_TIMEOUT: 
                lcd.clear();
                lcd.print("Time out error"); 
                delay(del);
                return;
    default: 
                lcd.clear();
                lcd.print("Unknown error"); 
                delay(del);
                return;
  }
  
  // Считываем с датчика BMP085:
  temperature = bmp085GetTemperature(bmp085ReadUT());
  temperature *= 0.1;
  pressure = bmp085GetPressure(bmp085ReadUP());
  pressure *= 0.01;
  
  // Разница давлений для вычисления простого прогноза
  weatherDiff = pressure - ePressure;
     
  rzero = gasSensor.getRZero();
  ppm = gasSensor.getPPM();
    
  // ЖК:
  lcd.setCursor(0, 0); // курсор на строку 1, поз. 1;
  //lcd.print("p = ");
  lcd.print(pressure*3/4); // Па -> мм рт.ст.
  lcd.print("mmHg "); // мм рт.ст.
 
  // "Прогноз":
  if(weatherDiff > 250)
    lcd.print("Sun");
  else if ((weatherDiff <= 250) || (weatherDiff >= -250))
    lcd.print("Cloudy");
  else if (weatherDiff > -250)
    lcd.print("Rain");
  
  lcd.setCursor(0, 1); // переход на строку 2
  //lcd.print("t=");
  lcd.print(temperature, 1);
  lcd.print("C ");
  
  //lcd.print("f=");
  lcd.print(sensorTempHumid.humidity);
  lcd.print("% ");
  
  lcd.print(ppm);

  //lcd.print("t=");
  //lcd.print(sensorTempHumid.temperature);
  //lcd.print("C ");
   
  delay(del);
  lcd.clear();
}

//  ВЫЧИСЛЕНИЕ ДАВЛЕНИЯ 

// Функция получает калибровочные значения для BMP085 
// и должна быть запущена в начале.
void bmp085Calibration()
{
  ac1 = bmp085ReadInt(0xAA);
  ac2 = bmp085ReadInt(0xAC);
  ac3 = bmp085ReadInt(0xAE);
  ac4 = bmp085ReadInt(0xB0);
  ac5 = bmp085ReadInt(0xB2);
  ac6 = bmp085ReadInt(0xB4);
  b1 = bmp085ReadInt(0xB6);
  b2 = bmp085ReadInt(0xB8);
  mb = bmp085ReadInt(0xBA);
  mc = bmp085ReadInt(0xBC);
  md = bmp085ReadInt(0xBE);
}

// Вычисление нескомпенсированной температуры.
// Возврашает значение в десятых долях градуса Цельсия.
short bmp085GetTemperature(unsigned int ut)
{
  long x1, x2;  
  x1 = (((long)ut - (long)ac6)*(long)ac5) >> 15;
  x2 = ((long)mc << 11)/(x1 + md);
  b5 = x1 + x2;
  return ((b5 + 8)>>4);  
}

/* Вычисление нескомпенсированного давления.
 Калибровочные значения должны быть уже известны.
 b5 также необходимо, поэтому сначала нужно вызвать bmp085GetTemperature().
 Возвращает значение в Паскалях. */
long bmp085GetPressure(unsigned long up)
{
  long x1, x2, x3, b3, b6, p;
  unsigned long b4, b7;
  
  b6 = b5 - 4000;
  
  // вычисление b3:
  x1 = (b2 * (b6 * b6)>>12)>>11;
  x2 = (ac2 * b6)>>11;
  x3 = x1 + x2;
  b3 = (((((long)ac1)*4 + x3)<<OSS) + 2)>>2;
  
  // вычисление b4:
  x1 = (ac3 * b6)>>13;
  x2 = (b1 * ((b6 * b6)>>12))>>16;
  x3 = ((x1 + x2) + 2)>>2;
  b4 = (ac4 * (unsigned long)(x3 + 32768))>>15;
  
  b7 = ((unsigned long)(up - b3) * (50000>>OSS));
  if (b7 < 0x80000000)
    p = (b7<<1)/b4;
  else
    p = (b7/b4)<<1;
    
  x1 = (p>>8) * (p>>8);
  x1 = (x1 * 3038)>>16;
  x2 = (-7357 * p)>>16;
  p += (x1 + x2 + 3791)>>4;
  
  return p;
}

// Чтение нескомпенсированного значения температуры.
unsigned int bmp085ReadUT()
{
  unsigned int ut;
  
  // Записывает 0x2E в регистр 0xF4
  // для запроса показаний температуры:
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x2E);
  Wire.endTransmission();  
  delay(5);
  
  // Читает 2 байта из регистров 0xF6 и 0xF7
  ut = bmp085ReadInt(0xF6);
  return ut;
}

// Чтение давления (нескомпенсированного).
unsigned long bmp085ReadUP()
{
  unsigned char msb, lsb, xlsb;
  unsigned long up = 0;
  
  // Записывает 0x34+(OSS<<6) в регистр 0xF4
  // для запроса показаний давления:
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x34 + (OSS<<6));
  Wire.endTransmission();
  
  // Ожидание преобразования, задержка зависит от OSS:
  delay(2 + (3<<OSS));
  
  // Чтение регистров 0xF6 (MSB), 0xF7 (LSB), 0xF8 (XLSB):
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF6);
  Wire.endTransmission();
  Wire.requestFrom(BMP085_ADDRESS, 3);
  
  // Ожидание данных:
  while(Wire.available() < 3);
  msb = Wire.read();
  lsb = Wire.read();
  xlsb = Wire.read();
  
  up = (((unsigned long) msb << 16) | ((unsigned long) lsb << 8) | (unsigned long) xlsb) >> (8-OSS);
  
  return up;
}

// Читает 1 байт из BMP085 по адресу 'address'.
char bmp085Read(unsigned char address)
{
  unsigned char data;
  
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();
  
  Wire.requestFrom(BMP085_ADDRESS, 1);
  while(!Wire.available());
    
  return Wire.read();
}

// Читает 2 байта из BMP085, начиная с адреса 'address'.
int bmp085ReadInt(unsigned char address)
{
  unsigned char msb, lsb;
  
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();
  
  Wire.requestFrom(BMP085_ADDRESS, 2);
  while(Wire.available()<2);
  msb = Wire.read();
  lsb = Wire.read();
  
  return (int) (msb<<8 | lsb);
}