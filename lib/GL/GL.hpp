/**
 * @author: Tevž Beškovnik
 * @date: 14. 7. 2022
 * @description: graphics library with basic render functions extends on the Adafruit-GFX
 * credit for a lot of these functions goes to them
*/

/*
TODO:
    - Implement the fast line drawing alg to as many functions as possible <--- (NEXT TO DO)
    - Fix texture rotation
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
#define MAX_TEX_BINDINGS 16

// texture bindings
#define TEXTURE_BINDING_0 0
#define TEXTURE_BINDING_1 1
#define TEXTURE_BINDING_2 2
#define TEXTURE_BINDING_3 3
#define TEXTURE_BINDING_4 4
#define TEXTURE_BINDING_5 5
#define TEXTURE_BINDING_6 6
#define TEXTURE_BINDING_7 7
#define TEXTURE_BINDING_8 8
#define TEXTURE_BINDING_9 9
#define TEXTURE_BINDING_10 10
#define TEXTURE_BINDING_11 11
#define TEXTURE_BINDING_12 12
#define TEXTURE_BINDING_13 13
#define TEXTURE_BINDING_14 14
#define TEXTURE_BINDING_15 15

#define CONTEXT_BUFFER 64

#define FONT_SIZE_1 1
#define FONT_SIZE_2 2
#define FONT_SIZE_3 3
#define FONT_SIZE_4 4

#define ROTATE_90 0
#define ROTATE_180 1
#define ROTATE_270 2

class GL : public Display
{
    public:
    GL(uint8_t clk, uint8_t mosi, uint8_t cs, uint16_t width, uint16_t height, uint32_t freq = 2000000);
    ~GL();

    void initGL();

    uint8_t* getContext();
    uint8_t* changeContext(uint8_t newContext = CONTEXT_BUFFER);

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
    bool drawTileFromMap(uint16_t x, uint16_t y, uint8_t tex_x, uint8_t tex_y);

    bool loadTexture(uint8_t* buffer, uint16_t width, uint16_t height, uint8_t textureBinding = TEXTURE_BINDING_0, bool dynmaci = false);
    bool allocateTexture(uint16_t width, uint16_t height, uint8_t textureBinding = TEXTURE_BINDING_0);
    bool saveToAllocated(uint8_t* buffer, uint8_t textureBinding = TEXTURE_BINDING_0);
    bool drawTexture(uint16_t x, uint16_t y, uint8_t textureBinding = TEXTURE_BINDING_0);
    void clearTexture(uint8_t textureBinding = TEXTURE_BINDING_0);

    void rotateTexture(uint8_t textureBinding = TEXTURE_BINDING_0, uint8_t rotation = ROTATE_90);

    bool cropTexture(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t textureBinding = TEXTURE_BINDING_0);
    bool cropTextureTo(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t textureBindingFrom = TEXTURE_BINDING_0, uint8_t textureBindingTo = TEXTURE_BINDING_1);

    void invertTexture(uint8_t textureBinding);

    bool blendAdd(uint16_t x, uint16_t y, uint8_t bindingSource, uint8_t bindingDest);
    bool blendSub(uint16_t x, uint16_t y, uint8_t bindingSource, uint8_t bindingDest);

    private:
    
    uint8_t* texture_buffer; // texture buffer to store tile maps
    uint16_t _texture_w; // height and width of the texture buffer
    uint16_t _texture_h;
    uint8_t _tile_w; // height and width of a tile in the tile map
    uint8_t _tile_h;
    uint8_t* tex[MAX_TEX_BINDINGS]; // textures theres 8 here that can be loaded at once; Probably gonna increase that number to like 32 or smtin
    uint16_t w[MAX_TEX_BINDINGS]; // width and height of the textures
    uint16_t h[MAX_TEX_BINDINGS];
    bool dynamicTex[MAX_TEX_BINDINGS]; // when clearing textures we goota know if theyre dynamicly allocated
    uint16_t _w; // frame buffer width and height
    uint16_t _h;

    //font stuff
    uint8_t _font_size; // font size for scaling
    uint8_t _text_color; // color of the text to be drawn
    uint8_t _text_bg_color; // backgorund color of the text if its the same as text color no bakground
    uint16_t _cursor_x; // x and y positon of the text cursor
    uint16_t _cursor_y;
    bool wrap; // text warpping
    bool _cp437; // ???? idk i stole this
};

#endif