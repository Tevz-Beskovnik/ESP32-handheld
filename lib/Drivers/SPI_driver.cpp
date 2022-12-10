#include <SPI_driver.hpp>

/*
    TODO: test out all this code (yes really this has to work)
*/

/**
 * @brief begins the SPI driver and initializes the SPI library set pin to -1 if not used
 * 
 * @param pinClk clk pin
 * 
 * @param pinMosi mosi pin
 * 
 * @param pinMiso miso pin
*/
SPIDriver::SPIDriver(uint8_t pinClk, int pinMosi, int pinMiso)
//: spi(new SPIClass()), settings(new SPISettings(clock, pBitOrder, SPI_MODE0))
{
    spi_bus_config_t cfg = {
        .mosi_io_num = pinMosi,
        .miso_io_num = pinMiso,
        .sclk_io_num = pinClk,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 0
    };

    bus_cfg = cfg;

    esp_err_t err = spi_bus_initialize(VSPI_HOST, &bus_cfg, 0);
    assert(err == ESP_OK);
}

// new spi device class for registering devices
/**
 * @description: initialize the SPI device
 * 
 * @param flags flags to add to the device configuration
 * 
 * @param clock clock speed at with the device operates
*/
SPIDevice::SPIDevice(uint8_t flags, int32_t clock)
{
    spi_device_interface_config_t cfg = {
        .mode = 0,
        .clock_speed_hz = (int)clock,
        .spics_io_num = -1,
        .flags = flags,
        .queue_size = 1,
        .pre_cb = nullptr,
    };

    dev_cfg = cfg;

    esp_err_t err = spi_bus_add_device(VSPI_HOST, &dev_cfg, &dev_handle);
    assert(err == ESP_OK);
}

/**
 * @description: issue command to spi device expect no return data
 * 
 * @param dataBuffer buffer of data to be sent ot the device
 * 
 * @param len number of bytes to be sent to the device
*/
void SPIDevice::spiCommand(uint8_t* dataBuffer, uint32_t len)
{
    spi_transaction_t transaction = {
        .flags = 0,
        .length = 8 * len, // size of char * lenght of buffer
        .tx_buffer = (void*)dataBuffer,
        .rx_buffer = NULL // we want no data back
    };

    dev_transaction = transaction;

    spi_device_transmit(dev_handle, &dev_transaction);
}

/**
 * @description: send command to device and read return into inBuffer
 * 
 * @param outBuffer buffer to be sent to device
 * 
 * @param outLen lenght of buffer in bytes that is being sent to device
 * 
 * @param inBuffer buffer to store returned data from device
 * 
 * @param inLen expected lenght of incoming data recieved from device
 * 
 * @param flags aditional flags to give to the transmit function
*/
void SPIDevice::spiCommand(uint8_t* outBuffer, uint32_t outLen, uint8_t* inBuffer, uint32_t inLen, uint32_t flags)
{
    spi_transaction_t transaction = {
        .flags = flags, // additional flags for the function
        .length = 8 * outLen, // sizeof(char) * (lenght of buffer)
        .rxlength = 8 * inLen,
        .tx_buffer = (void*)outBuffer,
        .rx_buffer = (void*)inBuffer
    };

    dev_transaction = transaction;

    esp_err_t err = spi_device_transmit(dev_handle, &dev_transaction);
    assert(err == ESP_OK);
}

/**
 * @description: send command to device and read return into inBuffer
 * 
 * @param outBuffer buffer to be sent to device
 * 
 * @param outLen lenght of buffer in bytes that is being sent to device
 * 
 * @param inBuffer buffer to store returned data from device
 * 
 * @param inLen expected lenght of incoming data recieved from device
*/
void SPIDevice::spiCommand(uint8_t* outBuffer, uint32_t outLen, uint8_t* inBuffer, uint32_t inLen)
{
    spi_transaction_t transaction = {
        .flags = 0, // additional flags for the function
        .length = 8 * outLen, // sizeof(char) * (lenght of buffer)
        .rxlength = 8 * inLen,
        .tx_buffer = (void*)outBuffer,
        .rx_buffer = (void*)inBuffer
    };

    dev_transaction = transaction;

    esp_err_t err = spi_device_transmit(dev_handle, &dev_transaction);
    assert(err == ESP_OK);
}

/**
 * @description: send previously transmited command to device
*/
void SPIDevice::spiCommand()
{
    esp_err_t err = spi_device_transmit(dev_handle, &dev_transaction);
    assert(err == ESP_OK);
}