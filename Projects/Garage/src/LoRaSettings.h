#pragma once

namespace LoRaSettings {
    const long FREQUENCY = 433E6;   // LoRa Frequency
    const int CS_PIN = 53;          // LoRa radio chip select
    const int RST_PIN = 22;          // LoRa radio reset
    const int IRQ_PIN = 23;          // (DIO0) must be a hardware interrupt pin
}
