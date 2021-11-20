#pragma once
#include <nRF24L01.h>                                          
#include <RF24.h>    

namespace Radio {
    static const int PIN_CE = 10;
    static const int PIN_CSN = 9;
    static const int CHANNEL = 97;

    static const uint64_t PIPE_0_ADDR = 0x45454545FF;
    static const uint64_t PIPE_1_ADDR = 0xC4C5C6C7F1;
    static const uint64_t PIPE_2_ADDR = 0xC4C5C6C7F2;
    
    enum ModuleType {
        None = 0,
        Receiver = 1,
        Transmitter
    };
}

RF24 RF24_Global(Radio::PIN_CSN, Radio::PIN_CE);

class NRF24Receiver {
public:

    NRF24Receiver(String id)
        :_id(id)
        , _nrf24(&RF24_Global)
        , _pipesCount(0)
    {}

    void Init() {
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
            _nrf24->setPALevel(RF24_PA_HIGH); 
        }
        else {
            Serial.println("NRF24Module is not inited");
        }         
    }

    void AddPipeForListening(uint64_t address) {
        _nrf24->openReadingPipe(_pipesCount, address); 
        _pipesCount++;    
        Serial.println("NRF24Receiver: Added pipe #" + String(_pipesCount));
    }

    void StartListeningAll() {
       _nrf24->startListening();       
       Serial.println("NRF24Receiver: Start listening all");
       Serial.println("NRF24Receiver: Pipes count = " + String(_pipesCount));
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
    size_t _pipesCount;
    static const int CAPACITOR_CHARGING_TIME = 1000;
};

namespace Radio {
    NRF24Receiver NRF24("NRF24.Receiver");
}
