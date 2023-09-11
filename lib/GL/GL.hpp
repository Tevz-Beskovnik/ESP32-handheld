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
#include <Active-Object.h>
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
    GL(uint8_t cs, uint16_t width, uint16_t height, int32_t freq = 2000000);
    ~GL();

    void initGL();

    uint8_t *getContext();
    uint8_t *changeContext(uint8_t newContext = CONTEXT_BUFFER);

    void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y);
    using Print::write; // tell the print library that we are writing our own write funtion
    virtual size_t write(uint8_t c);

    void setCursor(uint16_t x, uint16_t y)
    {
        _cursor_x = x;
        _cursor_y = y;
    };
    void fontSize(uint8_t size = FONT_SIZE_1) { _font_size = size; };
    void textColor(uint8_t color = BLACK)
    {
        _text_color = color;
        _text_bg_color = color;
    };
    void textColorBackground(uint8_t color = BLACK, uint8_t background = WHITE)
    {
        _text_color = color;
        _text_bg_color = background;
    };
    void fontWrap(uint8_t wrapOption = false) { wrap = wrapOption; };

    void drawFastRawHLine(uint16_t x, uint16_t y, uint16_t w, uint16_t color);
    void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
    void drawFastRawVLine(uint16_t x, uint16_t y, uint16_t h, uint16_t color);

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

    bool loadTileMap(uint8_t *buffer, uint16_t width, uint16_t height, uint8_t tile_w, uint8_t tile_h);
    bool loadTileFromMap(uint8_t x, uint8_t y, uint8_t textureBinding = TEXTURE_BINDING_0);
    bool drawTileFromMap(uint16_t x, uint16_t y, uint8_t tex_x, uint8_t tex_y);

    bool loadTexture(uint8_t *buffer, uint16_t width, uint16_t height, uint8_t textureBinding = TEXTURE_BINDING_0, bool dynmaci = false);
    bool allocateTexture(uint16_t width, uint16_t height, uint8_t textureBinding = TEXTURE_BINDING_0);
    bool saveToAllocated(uint8_t *buffer, uint8_t textureBinding = TEXTURE_BINDING_0);
    bool drawTexture(uint16_t x, uint16_t y, uint8_t textureBinding = TEXTURE_BINDING_0);
    void clearTexture(uint8_t textureBinding = TEXTURE_BINDING_0);

    void rotateTexture(uint8_t textureBinding = TEXTURE_BINDING_0, uint8_t rotation = ROTATE_90);

    bool cropTexture(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t textureBinding = TEXTURE_BINDING_0);
    bool cropTextureTo(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t textureBindingFrom = TEXTURE_BINDING_0, uint8_t textureBindingTo = TEXTURE_BINDING_1);

    void invertTexture(uint8_t textureBinding);

    bool blendAdd(uint16_t x, uint16_t y, uint8_t bindingSource, uint8_t bindingDest);
    bool blendSub(uint16_t x, uint16_t y, uint8_t bindingSource, uint8_t bindingDest);

private:
    uint8_t *texture_buffer; // texture buffer to store tile maps
    uint16_t _texture_w;     // height and width of the texture buffer
    uint16_t _texture_h;
    uint8_t _tile_w; // height and width of a tile in the tile map
    uint8_t _tile_h;
    uint8_t *tex[MAX_TEX_BINDINGS]; // textures theres 8 here that can be loaded at once; Probably gonna increase that number to like 32 or smtin
    uint16_t w[MAX_TEX_BINDINGS];   // width and height of the textures
    uint16_t h[MAX_TEX_BINDINGS];
    bool dynamicTex[MAX_TEX_BINDINGS]; // when clearing textures we goota know if theyre dynamicly allocated
    uint16_t _w;                       // frame buffer width and height
    uint16_t _h;

    // font stuff
    uint8_t _font_size;     // font size for scaling
    uint8_t _text_color;    // color of the text to be drawn
    uint8_t _text_bg_color; // backgorund color of the text if its the same as text color no bakground
    uint16_t _cursor_x;     // x and y positon of the text cursor
    uint16_t _cursor_y;
    bool wrap;   // text warpping
    bool _cp437; // ???? idk i stole this
};

// graphics events
#define PRINT 0
void ex_print(GL *gl, Event *ev)
{
    gl->print((const char *)ev->misc);
}

#define DRAW_CHAR 1
void ex_draw_char(GL *gl, Event *ev)
{
    gl->drawChar((int16_t)ev->params[0], (int16_t)ev->params[1], (uint8_t)ev->params[2], (uint16_t)ev->params[3], (uint16_t)ev->params[4], (uint8_t)ev->params[5], (uint8_t)ev->params[6]);
}

#define SET_CURSOR 2
void ex_set_cursor(GL *gl, Event *ev)
{
    gl->setCursor((uint16_t)ev->params[0], (uint16_t)ev->params[1]);
}

#define FONT_SIZE 3
void ex_font_size(GL *gl, Event *ev)
{
    gl->fontSize((uint8_t)ev->params[0]);
}

#define TEXT_COLOR 4
void ex_text_color(GL *gl, Event *ev)
{
    gl->textColor((uint8_t)ev->params[0]);
}

#define BACKGROUND_COLOR 5
void ex_background_color(GL *gl, Event *ev)
{
    gl->textColorBackground((uint8_t)ev->params[0], (uint8_t)ev->params[1]);
}

#define FONT_WRAP 6
void ex_font_wrap(GL *gl, Event *ev)
{
    gl->fontWrap((uint8_t)ev->params[0]);
}

#define DRAW_FAST_RAW_H_LINE 7
void ex_draw_fast_raw_h_line(GL *gl, Event *ev)
{
    gl->drawFastRawHLine((uint16_t)ev->params[0], (uint16_t)ev->params[1], (uint16_t)ev->params[2], (uint8_t)ev->params[3]);
}

#define DRAW_LINE 8
void ex_draw_line(GL *gl, Event *ev)
{
    gl->drawLine((uint16_t)ev->params[0], (uint16_t)ev->params[1], (uint16_t)ev->params[2], (uint16_t)ev->params[3], (uint16_t)ev->params[4]);
}

#define DRAW_FAST_RAW_V_LINE 9
void ex_draw_fast_raw_v_line(GL *gl, Event *ev)
{
    gl->drawFastRawVLine((uint16_t)ev->params[0], (uint16_t)ev->params[1], (uint16_t)ev->params[2], (uint16_t)ev->params[3]);
}

#define DRAW_RECT 10
void ex_draw_rect(GL *gl, Event *ev)
{
    gl->drawRect((uint16_t)ev->params[0], (uint16_t)ev->params[1], (uint16_t)ev->params[2], (uint16_t)ev->params[3], (uint16_t)ev->params[4]);
}

#define FILL_RECT 11
void ex_fill_rect(GL *gl, Event *ev)
{
    gl->fillRect((uint16_t)ev->params[0], (uint16_t)ev->params[1], (uint16_t)ev->params[2], (uint16_t)ev->params[3], (uint16_t)ev->params[4]);
}

#define DRAW_TRIANGLE 12
void ex_draw_triangle(GL *gl, Event *ev)
{
    gl->drawTriangle((uint16_t)ev->params[0], (uint16_t)ev->params[1], (uint16_t)ev->params[2], (uint16_t)ev->params[3], (uint16_t)ev->params[4], (uint16_t)ev->params[5], (uint16_t)ev->params[6]);
}

#define FILL_TRIANGLE 13
void ex_fill_triangle(GL *gl, Event *ev)
{
    gl->fillTriangle((uint16_t)ev->params[0], (uint16_t)ev->params[1], (uint16_t)ev->params[2], (uint16_t)ev->params[3], (uint16_t)ev->params[4], (uint16_t)ev->params[5], (uint16_t)ev->params[6]);
}

#define DRAW_CIRCLE 14
void ex_draw_circle(GL *gl, Event *ev)
{
    gl->drawCircle((uint16_t)ev->params[0], (uint16_t)ev->params[1], (uint8_t)ev->params[2], (uint8_t)ev->params[3]);
}

#define FILL_CIRCLE 15
void ex_fill_circle(GL *gl, Event *ev)
{
    gl->fillCircle((uint16_t)ev->params[0], (uint16_t)ev->params[1], (uint8_t)ev->params[2], (uint8_t)ev->params[3]);
}

#define DRAW_LINE_D 16
void ex_draw_line_d(GL *gl, Event *ev)
{
    gl->drawLineD((uint16_t)ev->params[0], (uint16_t)ev->params[1], (uint16_t)ev->params[2], (uint16_t)ev->params[3]);
}

#define DRAW_LINE_DI 17
void ex_draw_line_di(GL *gl, Event *ev)
{
    gl->drawLineDI((uint16_t)ev->params[0], (uint16_t)ev->params[1], (uint16_t)ev->params[2], (uint16_t)ev->params[3]);
}

#define DRAW_RECT_D 18
void ex_draw_rect_d(GL *gl, Event *ev)
{
    gl->drawRectD((uint16_t)ev->params[0], (uint16_t)ev->params[1], (uint16_t)ev->params[2], (uint16_t)ev->params[3]);
}

#define FILL_RECT_D 19
void ex_fill_rect_d(GL *gl, Event *ev)
{
    gl->fillRectD((uint16_t)ev->params[0], (uint16_t)ev->params[1], (uint16_t)ev->params[2], (uint16_t)ev->params[3]);
}

#define DRAW_TRIANGLE_D 20
void ex_draw_triangle_d(GL *gl, Event *ev)
{
    gl->drawTriangleD((uint16_t)ev->params[0], (uint16_t)ev->params[1], (uint16_t)ev->params[2], (uint16_t)ev->params[3], (uint16_t)ev->params[4], (uint16_t)ev->params[5]);
}

#define FILL_TRIANGLE_D 21
void ex_fill_triangle_d(GL *gl, Event *ev)
{
    gl->fillTriangleD((uint16_t)ev->params[0], (uint16_t)ev->params[1], (uint16_t)ev->params[2], (uint16_t)ev->params[3], (uint16_t)ev->params[4], (uint16_t)ev->params[5]);
}

#define LOAD_TILE_MAP 22
void ex_load_tile_map(GL *gl, Event *ev)
{
    gl->loadTileMap((uint8_t *)ev->misc, (uint16_t)ev->params[0], (uint16_t)ev->params[1], (uint8_t)ev->params[2], (uint8_t)ev->params[3]);
}

#define LOAD_TILE_FROM_MAP 23
void ex_load_tile_from_map(GL *gl, Event *ev)
{
    gl->loadTileFromMap((uint8_t)ev->params[0], (uint8_t)ev->params[1], (uint8_t)ev->params[2]);
}

#define DRAW_TILE_FROM_MAP 24
void ex_draw_tile_from_map(GL *gl, Event *ev)
{
    gl->drawTileFromMap((uint16_t)ev->params[0], (uint16_t)ev->params[1], (uint8_t)ev->params[2], (uint8_t)ev->params[3]);
}

#define LOAD_TEXTURE 25
void ex_load_texture(GL *gl, Event *ev)
{
    gl->loadTexture((uint8_t *)ev->misc, (uint16_t)ev->params[0], (uint16_t)ev->params[1], (uint8_t)ev->params[2], (bool)ev->params[3]);
}

#define ALLOCATE_TEXTURE 26
void ex_allocate_texture(GL *gl, Event *ev)
{
    gl->allocateTexture((uint16_t)ev->params[0], (uint16_t)ev->params[1], (uint8_t)ev->params[2]);
}

#define SAVE_TO_ALLOCATED 27
void ex_save_to_allocated(GL *gl, Event *ev)
{
    gl->saveToAllocated((uint8_t *)ev->misc, (uint8_t)ev->params[0]);
}

#define DRAW_TEXTURE 28
void ex_draw_texture(GL *gl, Event *ev)
{
    gl->drawTexture((uint16_t)ev->params[0], (uint16_t)ev->params[1], (uint8_t)ev->params[2]);
}

#define CLEAR_TEXTURE 29
void ex_clear_texture(GL *gl, Event *ev)
{
    gl->clearTexture((uint8_t)ev->params[0]);
}

#define ROTATE_TEXTURE 30
void ex_rotate_texture(GL *gl, Event *ev)
{
    gl->rotateTexture((uint8_t)ev->params[0], (uint8_t)ev->params[1]);
}

#define CROP_TEXTURE 31
void ex_crop_texture(GL *gl, Event *ev)
{
    gl->cropTexture((uint16_t)ev->params[0], (uint16_t)ev->params[1], (uint16_t)ev->params[2], (uint16_t)ev->params[3], (uint8_t)ev->params[4]);
}

#define CROP_TEXTURE_TO 32
void ex_crop_texture_to(GL *gl, Event *ev)
{
    gl->cropTextureTo((uint16_t)ev->params[0], (uint16_t)ev->params[1], (uint16_t)ev->params[2], (uint16_t)ev->params[3], (uint8_t)ev->params[4], (uint8_t)ev->params[5]);
}

#define INVERT_TEXTURE 33
void ex_invert_texture(GL *gl, Event *ev)
{
    gl->invertTexture((uint16_t)ev->params[0]);
}

#define BLEND_ADD 34
void ex_blend_add(GL *gl, Event *ev)
{
    gl->blendAdd((uint16_t)ev->params[0], (uint16_t)ev->params[1], (uint8_t)ev->params[2], (uint8_t)ev->params[3]);
}

#define BLEND_SUB 35
void ex_blend_sub(GL *gl, Event *ev)
{
    gl->blendSub((uint16_t)ev->params[0], (uint16_t)ev->params[1], (uint8_t)ev->params[2], (uint8_t)ev->params[3]);
}

#define GET_CONTEXT 36
void ex_get_context(GL *gl, Event *ev)
{
    uint8_t **ptr = (uint8_t **)ev->misc;

    *ptr = gl->getContext();
}

#define CHANGE_CONTEXT 37
void ex_change_context(GL *gl, Event *ev)
{
    uint8_t **ptr = (uint8_t **)ev->misc;

    *ptr = gl->changeContext((uint8_t)ev->params[0]);
}

bool graphics_dispatcher(Event *ev, void *args);

#endif