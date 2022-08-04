/**
 * @author: Tevž Beškovnik
 * @date: 14. 7. 2022
 * @description: graphics library with basic render functions extends on the Adafruit-GFX
 * credit for all other functions goes to them
*/

/*
TODO:
    - Add support for rotation of textures
*/

#ifndef _GL_
#define _GL_

#include <Arduino.h>
#include <Print.h>
#include <Display_driver.hpp>
#include <pgmspace.h>
#include <math.h>

/*
    - esp32 max memory 52k
*/
#ifndef DISPLAY_SIZE
#define DISPLAY_SIZE
#define DISPLAY_W 240
#define DISPLAY_H 120
#endif

#ifndef BLACK
#define BLACK 0
#endif

#ifndef WHITE
#define WHITE 1
#endif

// maximum amount of possible texture bindings
#define MAX_TEX_BINDINGS 8

// texture bindings
#define TEXTURE_BINDING_0 0
#define TEXTURE_BINDING_1 1
#define TEXTURE_BINDING_2 2
#define TEXTURE_BINDING_3 3
#define TEXTURE_BINDING_4 4
#define TEXTURE_BINDING_5 5
#define TEXTURE_BINDING_6 6
#define TEXTURE_BINDING_7 7

#define FONT_SIZE_1 1
#define FONT_SIZE_2 2
#define FONT_SIZE_3 3
#define FONT_SIZE_4 4

class GL : public Display
{
    public:
    GL(uint8_t clk, uint8_t mosi, uint8_t cs, uint16_t width, uint16_t height, uint32_t freq = 2000000);
    ~GL();

    void initGL();

    void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y);

    using Print::write;
    virtual size_t write(uint8_t c);

    void setCursor(uint16_t x, uint16_t y) { _cursor_x = x; _cursor_y = y; };
    void fontSize(uint8_t size = FONT_SIZE_1) { _font_size = size; };
    void textColor(uint8_t color = BLACK) { _text_color = color; _text_bg_color = color; };
    void textColorBackground(uint8_t color = BLACK, uint8_t background = WHITE) { _text_color = color; _text_bg_color = background; };
    void fontWrap(uint8_t wrapOption = false) { wrap = wrapOption; };

    void drawFastRawHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
    void drawFastRawVLine(int16_t x, int16_t y, int16_t h, uint16_t color);

    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

    void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
    void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);

    void drawCircle(uint16_t x, uint16_t y, uint8_t r, uint8_t color);
    void fillCircle(uint16_t x, uint16_t y, uint8_t r, uint8_t color);

    void drawLineD(uint16_t x, uint16_t y, uint16_t x1, uint16_t y1);
    void drawLineDI(uint16_t x, uint16_t y, uint16_t x1, uint16_t y1);

    void drawRectD(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void fillRectD(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

    void drawTriangleD(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3);
    void fillTriangleD(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3);

    bool loadTileMap(uint8_t* buffer, uint16_t width, uint16_t height, uint8_t tile_w, uint8_t tile_h);
    bool loadTileFromMap(uint8_t x, uint8_t y, uint8_t textureBinding = TEXTURE_BINDING_0);

    bool loadTexture(uint8_t* buffer, uint16_t width, uint16_t height, uint8_t textureBinding = TEXTURE_BINDING_0, bool dynmaci = false);
    bool drawTexture(uint16_t x, uint16_t y, uint8_t textureBinding = TEXTURE_BINDING_0);
    void clearTexture(uint8_t textureBinding = TEXTURE_BINDING_0);

    void cropTexture(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t textureBinding = TEXTURE_BINDING_0);

    private:
    // gl stuff
    uint8_t* context_buffer;
    uint8_t* texture_buffer;
    uint16_t _texture_w;
    uint16_t _texture_h;
    uint8_t _tile_w;
    uint8_t _tile_h;
    uint8_t* tex[8];
    uint16_t w[8];
    uint16_t h[8];
    bool dynamicTex[8];
    uint16_t _w;
    uint16_t _h;

    //font stuff
    uint8_t _font_size;
    uint8_t _text_color;
    uint8_t _text_bg_color;
    uint16_t _cursor_x;
    uint16_t _cursor_y;
    bool wrap;
    bool _cp437;
};

#endif