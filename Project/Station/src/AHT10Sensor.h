#pragma once
#include "Sensor.h"
#include <Wire.h>

class AHT10Sensor : public Sensor {
    typedef Sensor Super;

public:

    AHT10Sensor(String id)
        : Super(id)
    {}

    virtual void Init() {
        if (InitTransmition()) {
            _inited = true;
        }
    }

    String GetTemperature() {
        if (!_inited) {
            return Super::GetSensorNotInitializedMsg();
        }
        uint8_t  readedBytes, rc = false;
        uint8_t  rawData[0x06];
        uint32_t rawTemperature;

        Wire.beginTransmission(AHT10_I2C_ADDRESS);
        Wire.write(measureCmd, sizeof(measureCmd));
        Wire.endTransmission();

        while (!rc) {
            uint8_t statusByte;
            ReadStatus(statusByte);
            rc = !(statusByte & AHT10_STAT_BUSY);
            delay(10);
        }
        readedBytes = Wire.requestFrom(AHT10_I2C_ADDRESS, sizeof(rawData));
        if (sizeof(rawData) != readedBytes) {
            rc = false;
        }
        for (uint8_t i = 0; sizeof(rawData) > i; i++) {
            rawData[i] = Wire.read();
        }
        rawTemperature = (((uint32_t)rawData[3] & 0x0F) << 16) | ((uint32_t)rawData[4] << 8) | rawData[5];
        float temperature = (((float)rawTemperature * 200) / 1048576) - 50;

        if (rc) {
            return "Temperature = " + String(temperature) + " *C";
        }
        else {
            return GetID() + ": temperature reading error";
        }
    }

    String GetHumidity() {
        if (!_inited) {
            return Super::GetSensorNotInitializedMsg();
        }
        uint8_t  readedBytes, rc = false;
        uint8_t  rawData[0x06];
        uint32_t rawHumidity;

        Wire.beginTransmission(AHT10_I2C_ADDRESS);
        Wire.write(measureCmd, sizeof(measureCmd));
        Wire.endTransmission();

        while (!rc) {
            uint8_t statusByte;
            ReadStatus(statusByte);
            rc = !(statusByte & AHT10_STAT_BUSY);
            delay(10);
        }

        readedBytes = Wire.requestFrom(AHT10_I2C_ADDRESS, sizeof(rawData));
        if (sizeof(rawData) != readedBytes) {
            rc = false;
        }
        for (uint8_t i = 0; sizeof(rawData) > i; i++) {
            rawData[i] = Wire.read();
        }

        rawHumidity = (((uint32_t)rawData[1] << 16) | ((uint32_t)rawData[2] << 8) | rawData[3]) >> 4;
        float humidity = (float)rawHumidity * 100 / 1048576;
        if (rc) {
            return "Humidity = " + String(humidity) + "%";
        }
        else {
            return GetID() + ": humidity reading error";
        }
    }

private:

    uint8_t InitTransmition() {
        uint8_t statusByte, result;
        Wire.beginTransmission(AHT10_I2C_ADDRESS);
        Wire.write(initCmd, sizeof(initCmd));
        Wire.endTransmission();
        result = ReadStatus(statusByte);
        result &= ((statusByte & 0x68) == 0x08);
        return result;

    }

    uint8_t ReadStatus(uint8_t& _statusByte) {
        uint8_t readedBytes = Wire.requestFrom(AHT10_I2C_ADDRESS, 0x01);
        _statusByte = Wire.read();
        return (0x01 == readedBytes);
    }

private:
    const int AHT10_I2C_ADDRESS = 0x38;  // or 0x39
    const int AHT10_STAT_BUSY = 0x80;
    const uint8_t initCmd[3] = { 0xE1, 0x08, 0x00 }; // Init + CAL Enable
    const uint8_t measureCmd[3] = { 0xAC, 0x33, 0x00 };
    const uint8_t resetCmd = 0xBA; // The soft reset takes less than 20ms

};

namespace Sensors {
    AHT10Sensor AHT10("AHT10");
}
