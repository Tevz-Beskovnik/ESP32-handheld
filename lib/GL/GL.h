/**
 * @author: Tevž Beškovnik
 * @date: 14. 7. 2022
 * @description: graphics library with basic render functions extends on the Adafruit-GFX
 * credit for all other functions goes to them
*/

#ifndef _GL_
#define _GL_

#include <Arduino.h>
#include <Adafruit_SharpMem.h>

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

#ifdef MULTY_CORE_DRAW
void glCall(TaskFunction_t func, uint8_t* contextBuffer, uint8_t* texture, uint16_t x, uint16_t y, uint16_t tex_w, uint16_t tex_h);

void xDrawTexture(void* params);
#endif

class GL : public Adafruit_SharpMem
{
    public:
    GL(uint8_t clk, uint8_t mosi, uint8_t cs, uint16_t width, uint16_t height, uint32_t freq = 2000000);
    ~GL();

    void initGL();

    void drawLineD(uint16_t x, uint16_t y, uint16_t x1, uint16_t y1);
    void drawLineDI(uint16_t x, uint16_t y, uint16_t x1, uint16_t y1);

    void drawRectD(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void fillRectD(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

    void drawTriangleD(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3);
    void fillTriangleD(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3);

    bool loadTileMap(uint8_t* buffer, uint16_t width, uint16_t height, uint8_t tile_w, uint8_t tile_h);
    bool loadTileFromMap(uint8_t x, uint8_t y, uint8_t textureBinding = TEXTURE_BINDING_0);

    bool loadTexture(uint8_t* buffer, uint16_t width, uint16_t height, uint8_t textureBinding = TEXTURE_BINDING_0);
    bool drawTexture(uint16_t x, uint16_t y, uint8_t textureBinding = TEXTURE_BINDING_0);

    private: 
    uint8_t* context_buffer;
    uint8_t* texture_buffer;
    uint16_t _texture_w;
    uint16_t _texture_h;
    uint8_t _tile_w;
    uint8_t _tile_h;
    uint8_t* tex[8];
    uint16_t w[8];
    uint16_t h[8];
    uint16_t _w;
    uint16_t _h;
};

#endif