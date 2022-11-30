#ifndef _SPI_DRIVER_
#define _SPI_DRIVER_

#include <Wire.h>
#include <SPI.h>
#include <Arduino.h>

/**
 * TODO: Implement this shit that uses direct eps32 libs (line 317): 
 * https://github.com/Inteform/esp32-lora-library/blob/master/components/lora/lora.c
 * (should work way faster)
*/

class SPIDriver {
    public:
        SPIDriver(uint8_t pinClk, uint8_t pinMosi, uint8_t pinMiso, uint8_t pinCs, uint8_t pBitOrder, uint32_t clock);

        ~SPIDriver();

        void spiCommand(uint8_t* dataBuffer, uint32_t len);

        SPIClass* getInterface();

    private:
        SPIClass* spi;

        SPISettings* settings;
};

#endif