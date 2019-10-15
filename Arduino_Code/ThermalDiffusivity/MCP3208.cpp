#include "MCP3208.h"

void MCP3208::begin() {
    pinMode(_cs, OUTPUT);
    digitalWrite(_cs, HIGH);
    SPI.begin();
}

uint16_t MCP3208::read(uint8_t channel) {
    uint8_t mode = 0b00000110; // | ((channel & 0b111) << 2);
    uint8_t addr = ((channel & 0b111) << 6);
    digitalWrite(_cs, LOW);
    (void) SPI.transfer(mode);
    uint8_t b1 = SPI.transfer(addr);
    uint8_t b2 = SPI.transfer(0);
    digitalWrite(_cs, HIGH);

    return 0b0000111111111111 & ((b1 << 8) | (b2 >> 0));
}

// int16_t MCP3208::readDif(uint8_t channel) {
//     uint8_t diff;
//     uint8_t b1, b2;
//     uint8_t addr = 0b01000000 | ((channel & 0b11) << 3);
//     digitalWrite(_cs, LOW);
//     (void) SPI.transfer(addr);
//     b1 = SPI.transfer(0);
//     b2 = SPI.transfer(0);
//     digitalWrite(_cs, HIGH);

//     diff = (b1 << 4) | (b2 >> 4);
//     if (diff > 0) {
//         return diff;
//     }
//     addr = 0b01000100 | ((channel & 0b11) << 3);
//     digitalWrite(_cs, LOW);
//     (void) SPI.transfer(addr);
//     b1 = SPI.transfer(0);
//     b2 = SPI.transfer(0);
//     digitalWrite(_cs, HIGH);
//     diff = (b1 << 4) | (b2 >> 4);
//     return -diff;
// }