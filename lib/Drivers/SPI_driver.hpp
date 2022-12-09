#ifndef _SPI_DRIVER_
#define _SPI_DRIVER_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "soc/gpio_struct.h"
#include "driver/gpio.h"

/**
 * TODO: Implement this shit that uses direct eps32 libs (line 317): 
 * https://github.com/Inteform/esp32-lora-library/blob/master/components/lora/lora.c
 * (should work way faster)
 * display running at 2Mhz
*/

class SPIDriver {
    public:
        SPIDriver(uint8_t pinClk, int pinMosi, int pinMiso);

    private:
        spi_bus_config_t bus_cfg;
};

// new spi devcie class
class SPIDevice {
    public:
        SPIDevice(uint8_t flags, int32_t clock = 2000000);

        void spiCommand(uint8_t* dataBuffer, uint32_t len);

        void spiCommand(uint8_t* outBuffer, uint32_t outLen, uint8_t* inBuffer, uint32_t inLen);

        void spiCommand();

    private:
        // device config
        spi_device_interface_config_t dev_cfg;
        // device handle
        spi_device_handle_t dev_handle;
        // transaction data
        spi_transaction_t dev_transaction;
};

#endif