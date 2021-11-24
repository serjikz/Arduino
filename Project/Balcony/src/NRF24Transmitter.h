#pragma once
#include <nRF24L01.h>                                          
#include <RF24.h>    

namespace Radio {
    static const int PIN_CE = 10;
    static const int PIN_CSN = 9;
    static const uint8_t CHANNEL = 0x6f;

    static const uint32_t PIPE_0_ADDR = 111156789;
    static const uint64_t PIPE_1_ADDR = 0xC4C5C6C7F1;
    static const uint64_t PIPE_2_ADDR = 0xC4C5C6C7F2;
    
    enum ModulePosition {
        Balcony = 0,
        Hall = 1,
        Garage
    };   

    enum SensorsCount {
        One = 1,
        Two = 2,
        Three,
        Four,
        Five,
        Six
    };


    static const int SIZE = 5;
    float data[SIZE];

    static const float HASH = 3.1415926f;
}

RF24 RF24_Global(Radio::PIN_CSN, Radio::PIN_CE);

class NRF24Transmitter {
public:

    NRF24Transmitter(String id)
        :_id(id)
        , _nrf24(&RF24_Global)
    {}

    void Init(uint64_t address) {
        delay(CAPACITOR_CHARGING_TIME);
        if (!_nrf24) {
            return;
        }
        _inited = false;
        _nrf24->begin();
        if (_nrf24->isPVariant() && _nrf24->isValid()) {
            _inited = true;
            Serial.println("Inited " + GetID());
            _nrf24->setChannel(Radio::CHANNEL); 
            _nrf24->setDataRate(RF24_1MBPS);
            _nrf24->setCRCLength(RF24_CRC_8);
            _nrf24->setPALevel(RF24_PA_MAX); 
            _nrf24->setAutoAck(false);       // автоответ
            _nrf24->powerUp();               // включение или пониженное потребление powerDown - powerUp
            _nrf24->stopListening();  //радиоэфир не слушаем, только передача    
            _nrf24->openWritingPipe(address);
            _nrf24->printDetails();
        }
        else {
            Serial.println("NRF24Module is not inited");
        }         
    }

    void TransmitData() {
        bool transmitted = _nrf24->write(Radio::data, sizeof(Radio::data));
        if (transmitted)
        {
            Serial.println("Data transmitted, size = " + String(sizeof(Radio::data)));
        }
        else {
            Serial.println("Data is not transmitted");
        }
    }

    String GetID() const {
        if (_inited) {
            return _id;
        }
        else {
            Serial.println("NRF24Module is not inited");
        }
    }

private:
    RF24* _nrf24;   
    String _id;
    bool _inited;
    static const int CAPACITOR_CHARGING_TIME = 1000;
};

namespace Radio {
    NRF24Transmitter NRF24("NRF24.Transmitter.Balcony");
}
