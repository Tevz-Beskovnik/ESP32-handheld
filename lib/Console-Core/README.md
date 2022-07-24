/**
 * @author: Tevž Beškovnik
 * @date: 23. 7. 2022
 * @description: handles everything to do with logic of game console, drawing, game loop ect....
*/
#pragma once

#include <GL.h>
#include <FreeRTOSConfig.h>
#include <esp_task_wdt.h>
#include <dataHeap.h>
#include <math.h>

/*
TODO:
    - Handle drawing on second thread
    - Create even handler 
    - Use event handler on drawing thread
    - Main thread used for console interface and game loop
*/

/*
    implement semaphor
    DRAW THREAD: (2nd thread)
    checks the draw stack to see if anything is to be drawn (use semaphore to push and pull from it),
    draw the according instruction to the display,
    handle all things with textures ect...

    CONSOLE THREAD: (main thread)
    handle GUI logic,
    handle game logic,
*/

/*========================
    DRAW INSTRUCTIONS
========================*/
// no operation equiv to empty function
#define NOOP                    0b000000

// regulad draw instructions
#define DRAW_RECT               0b010100
#define DRAW_RECTD              0b000001
#define FILL_RECT               0b000010
#define FILL_RECTD              0b000011
#define DRAW_POLY               0b000101
#define DRAW_POLYD              0b000110
#define FILL_POLY               0b000111
#define FILL_POLYD              0b001000
#define DRAW_LINE               0b001001
#define DRAW_LINED              0b001010
#define DRAW_LINEDI             0b001011

// draw texture from tex binding n
#define DRAW_TEX0               0b001100
#define DRAW_TEX1               0b010011
#define DRAW_TEX2               0b001101
#define DRAW_TEX3               0b001110
#define DRAW_TEX4               0b001111
#define DRAW_TEX5               0b010000
#define DRAW_TEX6               0b010001
#define DRAW_TEX7               0b010010

// load texture from tilemap
#define LD_TEX0                 0b010101
#define LD_TEX1                 0b010110
#define LD_TEX2                 0b010111
#define LD_TEX3                 0b011000
#define LD_TEX4                 0b011001
#define LD_TEX5                 0b011010
#define LD_TEX6                 0b011011
#define LD_TEX7                 0b011100

// text instructions
#define DRAW_TEXT               0b011101

// general display instructions
#define CLEAR_DISPLAY           0b011110
#define CLEAR_DISPLAY_BUFFER    0b011111
#define REFRESH_DISPLAY         0b100000

#define NUM_INSTRUCTIONS 33

typedef void(*GameLoops_t[4])();

typedef void(*Game_t)();

typedef void(*DrawInstructionSet_t[NUM_INSTRUCTIONS])(DrawInstruction);

TaskHandle_t drawTask;

SemaphoreHandle_t instructionHeapSemaphore = xSemaphoreCreateBinary();

Node_heap_s instructionHeap;

GameLoops_t gameLoops;

char* gameNames[4];

uint8_t numGames = 0;

DrawInstructionSet_t instructions;

uint16_t viewportW, viewportH;

GL* gl;

/*===============================
    ACTUAL DRAW INSTRUCTIONS
===============================*/

void noop(DrawInstruction d){ asm("nop"); };

void drawRect(DrawInstruction d) { gl->drawRect(d.x0, d.y0, abs(d.x0-d.x1), abs(d.y0-d.y1), d.color); };

void drawRectD(DrawInstruction d) { gl->drawRectD(d.x0, d.y0, abs(d.x0-d.x1), abs(d.y0-d.y1)); };

void fillRect(DrawInstruction d) { gl->fillRect(d.x0, d.y0, abs(d.x0-d.x1), abs(d.y0-d.y1), d.color); };

void fillRectD(DrawInstruction d) { gl->fillRectD(d.x0, d.y0, abs(d.x0-d.x1), abs(d.y0-d.y1)); };

void drawPoly(DrawInstruction d) { gl->drawTriangle(d.x0, d.y0, d.x1, d.y1, d.x2, d.y2, d.color); };

void drawPolyD(DrawInstruction d) { gl->drawTriangleD(d.x0, d.y0, d.x1, d.y1, d.x2, d.y2); };

void fillPoly(DrawInstruction d) { gl->fillTriangle(d.x0, d.y0, d.x1, d.y1, d.x2, d.y2, d.color); };

void fillPolyD(DrawInstruction d) { gl->fillTriangleD(d.x0, d.y0, d.x1, d.y1, d.x2, d.y2); };

void drawLine(DrawInstruction d) { gl->drawLine(d.x0, d.y0, d.x1, d.y1, d.color); };

void drawLineD(DrawInstruction d) { gl->drawLineD(d.x0, d.y0, d.x1, d.y1 ); };

void drawLineDI(DrawInstruction d) { gl->drawLineDI(d.x0, d.y0, d.x1, d.y1 ); };

void drawTex0(DrawInstruction d) { gl->drawTexture(d.x0, d.y0, TEXTURE_BINDING_0); };

void drawTex1(DrawInstruction d) { gl->drawTexture(d.x0, d.y0, TEXTURE_BINDING_1); };

void drawTex2(DrawInstruction d) { gl->drawTexture(d.x0, d.y0, TEXTURE_BINDING_2); };

void drawTex3(DrawInstruction d) { gl->drawTexture(d.x0, d.y0, TEXTURE_BINDING_3); };

void drawTex4(DrawInstruction d) { gl->drawTexture(d.x0, d.y0, TEXTURE_BINDING_4); };

void drawTex5(DrawInstruction d) { gl->drawTexture(d.x0, d.y0, TEXTURE_BINDING_5); };

void drawTex6(DrawInstruction d) { gl->drawTexture(d.x0, d.y0, TEXTURE_BINDING_6); };

void drawTex7(DrawInstruction d) { gl->drawTexture(d.x0, d.y0, TEXTURE_BINDING_7); };

void ldTex0(DrawInstruction d) { gl->loadTileFromMap(d.x0, d.y0, TEXTURE_BINDING_0 ); };

void ldTex1(DrawInstruction d) { gl->loadTileFromMap(d.x0, d.y0, TEXTURE_BINDING_1 ); };

void ldTex2(DrawInstruction d) { gl->loadTileFromMap(d.x0, d.y0, TEXTURE_BINDING_2 ); };

void ldTex3(DrawInstruction d) { gl->loadTileFromMap(d.x0, d.y0, TEXTURE_BINDING_3 ); };

void ldTex4(DrawInstruction d) { gl->loadTileFromMap(d.x0, d.y0, TEXTURE_BINDING_4 ); };

void ldTex5(DrawInstruction d) { gl->loadTileFromMap(d.x0, d.y0, TEXTURE_BINDING_5 ); };

void ldTex6(DrawInstruction d) { gl->loadTileFromMap(d.x0, d.y0, TEXTURE_BINDING_6 ); };

void ldTex7(DrawInstruction d) { gl->loadTileFromMap(d.x0, d.y0, TEXTURE_BINDING_7 ); };

void drawText(DrawInstruction d) { gl->setTextColor(d.color); gl->setCursor(d.x0, d.y0); gl->print(d.text); };

void clearDisplay(DrawInstruction d) { gl->clearDisplay(); };

void clearDisplayBuffer(DrawInstruction d) { gl->clearDisplayBuffer(); };

void refreshDisplay(DrawInstruction d) { gl->refresh(); };

/*=======================
    CONSOLE FUNCTIONS
=======================*/

/**
 * @brief loop for the thread that handles drawing
 * 
 * @param params params for function when its opened to new core
*/
void drawLoop(void* params)
{
    while(true)
    {
        if(xSemaphoreTake(instructionHeapSemaphore, portMAX_DELAY) == pdTRUE)
        {
            DrawInstruction instruction = pop(&instructionHeap);
            instructions[instruction.ins](instruction);
            xSemaphoreGive(instructionHeapSemaphore);
            yield();
        }
    }
}

/**
 * @brief initializes the console core
 * 
 * @param clk clk pin for sharp memory display
 * 
 * @param di di pin for the sharp memory display
 * 
 * @param cs cs pin for sharp memory display
 * 
 * @param screenW screen widht of the display
 * 
 * @param screenH screen height of the display
*/
void initConsole(uint8_t clk, uint8_t di, uint8_t cs, uint16_t screenW, uint16_t screenH)
{
    gl = new GL(clk, di, cs, screenW, screenH);

    gl->initGL();
    gl->clearDisplay();

    viewportW = screenW;
    viewportH = screenH;

    // registering the draw instructions
    instructions[NOOP] = noop;

    if(instructionHeapSemaphore == NULL)
        while(true);

    xSemaphoreGive(instructionHeapSemaphore);

    // regular instructions
    instructions[DRAW_RECT] = drawRect;
    instructions[DRAW_RECTD] = drawRectD;
    instructions[FILL_RECT] = fillRect;
    instructions[FILL_RECTD] = fillRectD;
    instructions[DRAW_POLY] = drawPoly;
    instructions[DRAW_POLYD] = drawPolyD;
    instructions[FILL_POLY] = fillPoly;
    instructions[FILL_POLYD] = fillPoly;
    instructions[DRAW_LINE] = drawLine;
    instructions[DRAW_LINED] = drawLineD;
    instructions[DRAW_LINEDI] = drawLineDI;

    // texture drawing
    instructions[DRAW_TEX0] = drawTex0;
    instructions[DRAW_TEX1] = drawTex1;
    instructions[DRAW_TEX2] = drawTex2;
    instructions[DRAW_TEX3] = drawTex3;
    instructions[DRAW_TEX4] = drawTex4;
    instructions[DRAW_TEX5] = drawTex5;
    instructions[DRAW_TEX6] = drawTex6;
    instructions[DRAW_TEX7] = drawTex7;
    
    // texture loading
    instructions[LD_TEX0] = ldTex0;
    instructions[LD_TEX1] = ldTex1;
    instructions[LD_TEX2] = ldTex2;
    instructions[LD_TEX3] = ldTex3;
    instructions[LD_TEX4] = ldTex4;
    instructions[LD_TEX5] = ldTex5;
    instructions[LD_TEX6] = ldTex6;
    instructions[LD_TEX7] = ldTex7;

    // text drawing
    instructions[DRAW_TEXT] = drawText;

    // general display instructions
    instructions[CLEAR_DISPLAY] = clearDisplay;
    instructions[CLEAR_DISPLAY_BUFFER] = clearDisplayBuffer;
    instructions[REFRESH_DISPLAY] = refreshDisplay;

    xTaskCreatePinnedToCore(drawLoop, "drawTask", 10000, NULL, 7, &drawTask, 0);
}

/**
 * @brief registers game loop to be used
 * 
 * @param loop game loop to use in the console max num of games registered is 4 currently
*/
bool registerGame(Game_t game, const char* name)
{
    if(numGames < 4)
    {
        gameLoops[numGames] = game;
        gameNames[numGames] = (char*)name;
        numGames++;
        return true;
    }
    return false;
}

/**
 * @brief set tilemap to be used when loading textures and drawing textures
 * 
 * @param map pointer to buffer containing tilemap
 * 
 * @param width tilemap width
 * 
 * @param height tilemap height
 * 
 * @param tileWidth tile width
 * 
 * @param tileHeight tile height
*/
void setTileMap(uint8_t* map, uint16_t width, uint16_t height, uint8_t tileWidth, uint8_t tileHeight)
{
    gl->loadTileMap(map, width, height, tileWidth, tileHeight);
}

/**
 * @brief logic of the console
*/
void consoleLoop()
{
    gameLoops[0]();
}

/*================================
    ABSTRACTED CALLING SYSTEM
================================*/

/**
 * @brief calls a graphics library instruction
*/
void glCall(uint8_t instruction, int16_t x, int16_t y, int16_t x1, int16_t y1, bool color)
{
    if(xSemaphoreTake(instructionHeapSemaphore, portMAX_DELAY) == pdTRUE)
    {
        push(&instructionHeap, {instruction, x, y, x1, y1, 0, 0, color, 0});
        xSemaphoreGive(instructionHeapSemaphore);
    }
}

/**
 * @brief calls a graphics library instruction
*/
void glCall(uint8_t instruction, int16_t x, int16_t y, int16_t x1, int16_t y1)
{
    if(xSemaphoreTake(instructionHeapSemaphore, portMAX_DELAY) == pdTRUE) 
    {
        push(&instructionHeap, {instruction, x, y, x1, y1, 0, 0, 0, 0});
        xSemaphoreGive(instructionHeapSemaphore);
    }
}

/**
 * @brief calls a graphics library instruction
*/
void glCall(uint8_t instruction, int16_t x, int16_t y, int16_t x1, int16_t y1, int16_t x2, int16_t y2, bool color)
{
    if(xSemaphoreTake(instructionHeapSemaphore, portMAX_DELAY) == pdTRUE)
    {
        push(&instructionHeap, {instruction, x, y, x1, y1, x2, y2, color, 0});
        xSemaphoreGive(instructionHeapSemaphore);
    }
}

/**
 * @brief calls a graphics library instruction
*/
void glCall(uint8_t instruction, int16_t x, int16_t y, int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
    if(xSemaphoreTake(instructionHeapSemaphore, portMAX_DELAY) == pdTRUE)
    {
        push(&instructionHeap, {instruction, x, y, x1, y1, x2, y2, 0, 0});
        xSemaphoreGive(instructionHeapSemaphore);
    }
}

/**
 * @brief calls a graphics library instruction
*/
void glCall(uint8_t instruction)
{
    if(xSemaphoreTake(instructionHeapSemaphore, portMAX_DELAY) == pdTRUE)
    {
        push(&instructionHeap, {instruction, 0, 0, 0, 0, 0, 0, 0, 0});
        xSemaphoreGive(instructionHeapSemaphore);
    }
}

/**
 * @brief calls a graphics library instruction
*/
void glCall(uint8_t instruction, int16_t x, int16_t y)
{
    if(xSemaphoreTake(instructionHeapSemaphore, portMAX_DELAY) == pdTRUE)
    {
        push(&instructionHeap, {instruction, x, y, 0, 0, 0, 0, 0, 0});
        xSemaphoreGive(instructionHeapSemaphore);
    }
}

/**
 * @brief calls a graphics library instruction
*/
void glCall(uint8_t instruction, int16_t x, int16_t y, const char* text, bool color)
{
    if(xSemaphoreTake(instructionHeapSemaphore, portMAX_DELAY) == pdTRUE)
    {
        push(&instructionHeap, {instruction, x, y, 0, 0, 0, 0, color, (char*)text});
        xSemaphoreGive(instructionHeapSemaphore);
    }
}
