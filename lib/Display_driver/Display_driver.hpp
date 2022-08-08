#ifndef _DISPLAY_DRIVER_
#define _DISPLAY_DRIVER_

#ifndef BLACK
#define BLACK 0
#endif

#ifndef WHITE
#define WHITE 1
#endif

#include <Print.h>
#include <SPI_driver.hpp>

class Display : public Print {
    public:
        virtual size_t write(uint8_t c) { return 0; };

        Display(uint8_t cs, uint8_t clk, uint8_t mosi, uint32_t freq, uint16_t width, uint16_t height);

        ~Display();

        void begin();

        void clearDisplay();

        void clearDisplayBuffer();

        void drawPixel(uint16_t x, uint16_t y, uint8_t color);

        void refresh();

        void refresh(uint8_t line);

    protected:
        uint8_t* display_buffer;
        uint8_t* context_buffer; // context buffer; It's basicly the frame buffer of the screen

    private:
        SPIDriver* spi_interface;

        uint8_t sharpmem_vcom;

        uint8_t bytes_per_line;

        uint8_t csPin;

        uint16_t width;
        uint16_t height;
        
};

#endif