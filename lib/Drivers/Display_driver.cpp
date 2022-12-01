#include <Display_driver.hpp>

// sharp memory display commands
#define SHARPMEM_CMD_WRITE_LINE 0b00000001
#define SHARPMEM_CMD_VCOM 0b00000010
#define SHARPMEM_CMD_CLEAR_SCREEN 0b00000100

#define TOGGLE_VCOM                                                              \
  	do {                                                                         \
    	sharpmem_vcom = sharpmem_vcom ? 0x00 : SHARPMEM_CMD_VCOM;                \
	} while (0);

// 1<<n is a costly operation on AVR -- table usu. smaller & faster
static const uint8_t PROGMEM set[] = {1, 2, 4, 8, 16, 32, 64, 128},
                             clr[] = {(uint8_t)~1,  (uint8_t)~2,  (uint8_t)~4,
                                      (uint8_t)~8,  (uint8_t)~16, (uint8_t)~32,
                                      (uint8_t)~64, (uint8_t)~128};

/**
 * @brief constructor of the display driver
 * 
 * @param cs cs pin
 * 
 * @param clk clk pin
 * 
 * @param mosi mosi pin
 * 
 * @param freq spi frequency
 * 
 * @param screen_width display width
 * 
 * @param screen_height display height
*/
Display::Display(uint8_t cs, uint32_t freq, uint16_t screen_width, uint16_t screen_height)
    : device(new SPIDevice(SPI_LSBFIRST, freq)), sharpmem_vcom(SHARPMEM_CMD_VCOM), height(screen_height), width(screen_width), csPin(cs), bytes_per_line(screen_width/8)
{
}

Display::~Display() { free(display_buffer); }

/**
 * @brief initializes the library 
*/
void Display::begin()
{
    display_buffer = (uint8_t*)malloc(sizeof(uint8_t)*height*width/8);
    memset((void*)display_buffer, 0xFF, sizeof(uint8_t)*height*width/8);

    digitalWrite(csPin, LOW);
}

/**
 * @brief clear display
*/
void Display::clearDisplay()
{
    digitalWrite(csPin, HIGH);
    uint8_t data[2] = {(uint8_t)(sharpmem_vcom | SHARPMEM_CMD_CLEAR_SCREEN), 0};
    device->spiCommand(data, 2);
    TOGGLE_VCOM
    digitalWrite(csPin, LOW);
}

/**
 * @brief clear display buffer
*/
void Display::clearDisplayBuffer()
{
    memset((void*)display_buffer, 0xFFFFFFFF, sizeof(uint8_t)*height*width/8);
}

/**
 * @brief draw pixel to display buffer at x y cooridnates
 * 
 * @param x coordinate to draw to
 * 
 * @param y cooridnate to draw to
 * 
 * @param color to draw
*/
void Display::drawPixel(uint16_t x, uint16_t y, uint8_t color)
{
    if (color) {
        context_buffer[(y * width + x) / 8] |= pgm_read_byte(&set[x & 7]);
    } else {
        context_buffer[(y * width + x) / 8] &= pgm_read_byte(&clr[x & 7]);
    }
}

/**
 * @brief draw the display buffer to display
*/
void Display::refresh()
{
    uint8_t cmd = sharpmem_vcom | SHARPMEM_CMD_WRITE_LINE;
    uint8_t line[bytes_per_line+2];
    uint16_t totalBytes = (width*height/8);

    digitalWrite(csPin, HIGH);
    device->spiCommand(&cmd, 1);

    for(uint16_t i = 0; i < totalBytes; i+=bytes_per_line){
        line[0] = (i/bytes_per_line)+1;
        line[bytes_per_line+1] = 0x00;

        memcpy((uint8_t*)(line+1), (uint8_t*)(display_buffer+i), bytes_per_line);

        device->spiCommand(line, bytes_per_line+2);
    }

    cmd = 0x00;
    device->spiCommand(&cmd, 1);
    TOGGLE_VCOM
    digitalWrite(csPin, LOW);
}

/**
 * @brief refresh display with display buffer at line number
 * 
 * @param lineNum number of line to draw to
*/
void Display::refresh(uint8_t lineNum)
{
    uint16_t lineAddress = lineNum*bytes_per_line;
    uint8_t line[bytes_per_line+4];

    line[0] = sharpmem_vcom | SHARPMEM_CMD_WRITE_LINE;
    line[1] = lineNum;
    line[bytes_per_line+2] = 0x00;
    line[bytes_per_line+3] = 0x00;

    memcpy((uint8_t*)((uint8_t*)(line)+2), (uint8_t*)(display_buffer+lineAddress), bytes_per_line);

    digitalWrite(csPin, HIGH); // <---- Data transmition
    device->spiCommand(line, bytes_per_line+4);
    TOGGLE_VCOM
    digitalWrite(csPin, LOW);
}