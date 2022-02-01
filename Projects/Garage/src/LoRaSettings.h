#pragma once

namespace LoRaSettings {
    const long FREQUENCY = 433E6;   // LoRa Frequency
    const int CS_PIN = 10;          // LoRa radio chip select
    const int RST_PIN = 9;          // LoRa radio reset
    const int IRQ_PIN = 2;          // change for your board; must be a hardware interrupt pin
}
