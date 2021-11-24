#pragma once
#include <nRF24L01.h>                                          
#include <RF24.h>    

namespace Radio {
    static const int PIN_CE = 9;
    static const int PIN_CSN = 10;
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

    float receivedData[SIZE];
    float jsonData[SIZE];

    void CopyDataToJson() {
        for (size_t i = 0; i < SIZE; i++) {
            jsonData[i] = receivedData[i];
        }
    }

    void ClearReceivedData() {
        for (size_t i = 0; i < SIZE; i++) {
            receivedData[i] = 0.f;
        }
    }

    static const float HASH = 3.1415926f;
    static const float EPS = 0.001f;
    static const short int MAX_RECEIVING_ATTEMPTS = 20000;
    static int receivngAttempts = 0;
}

RF24 RF24_Global(Radio::PIN_CE, Radio::PIN_CSN);

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
            _nrf24->setPALevel(RF24_PA_MAX); 
            _nrf24->setDataRate(RF24_1MBPS); 
            _nrf24->setCRCLength(RF24_CRC_8); 
            _nrf24->setChannel(Radio::CHANNEL);         
            _nrf24->setAutoAck(false);
            _nrf24->printDetails();
        }
        else {
            Serial.println("NRF24Module is not inited");
        }         
    }

    void AddPipeForListening(uint64_t address) {
        _nrf24->openReadingPipe(_pipesCount, Radio::PIPE_0_ADDR);
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

    bool IsAvailable() {
        if (_nrf24 && _inited) {
            return _nrf24->available();
        }
        else {
            return false;
        }
    }

    void ReceiveData() {        
        _nrf24->read(&Radio::receivedData, sizeof(Radio::receivedData));
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
