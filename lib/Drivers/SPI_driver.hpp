#ifndef _SPI_DRIVER_
#define _SPI_DRIVER_

#include <Wire.h>
#include <SPI.h>
#include <Arduino.h>

class SPIDriver {
    public:
        SPIDriver(uint8_t pinClk, uint8_t pinMosi, uint8_t pinCs, uint8_t pBitOrder, uint32_t clock);

        ~SPIDriver();

        void spiCommand(uint8_t* dataBuffer, uint32_t len);

    private:
        SPIClass* spi;

        SPISettings* settings;
};

#endif