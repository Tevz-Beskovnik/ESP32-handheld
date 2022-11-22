#include <SPI_driver.hpp>

/**
 * @brief begins the SPI driver and initializes the SPI library
 * 
 * @param pinClk clk pin
 * 
 * @param pinMosi mosi pin
 * 
 * @param pinCs cs pin
 * 
 * @param pBitOrder bit order of data
 * 
 * @param clock frequency 
*/
SPIDriver::SPIDriver(uint8_t pinClk, uint8_t pinMosi, uint8_t pinMiso, uint8_t pinCs, uint8_t pBitOrder, uint32_t clock)
: spi(new SPIClass()), settings(new SPISettings(clock, pBitOrder, SPI_MODE0))
{
    spi->begin(pinClk, pinMiso, pinMosi, pinCs);

    pinMode(pinCs, OUTPUT);
}

SPIDriver::~SPIDriver() { delete spi; delete settings; }

/**
 * @brief writes command to spi
 * 
 * @param dataBuffer data buffer to be sent
 * 
 * @param lan length of buffer
*/
void SPIDriver::spiCommand(uint8_t* dataBuffer, uint32_t len)
{
    spi->beginTransaction(*settings);
    spi->transfer(dataBuffer, len);
    spi->endTransaction();
}

SPIClass* SPIDriver::getInterface()
{
    return spi;
}