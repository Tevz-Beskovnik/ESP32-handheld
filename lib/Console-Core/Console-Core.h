/**
 * @author: Tevž Beškovnik
 * @date: 23. 7. 2022
 * @description: handles everything to do with logic of game console, drawing, game loop ect....
*/

#ifndef _CONSOLE_
#define _CONSOLE_

#include <GL.h>
#include <FreeRTOSConfig.h>
#include <dataHeap.h>

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

typedef void(*GameLoop_t)();

typedef void(*DrawCall_t)(DrawInstruction);

// draw instructions
#define NOOP            0b000000
#define DRAW_RECT       0b010100
#define DRAW_RECTD      0b000001
#define FILL_RECT       0b000010
#define FILL_RECTD      0b000011
#define DRAW_POLY       0b000101
#define DRAW_POLYD      0b000110
#define FILL_POLY       0b000111
#define FILL_POLYD      0b001000
#define DRAW_LINE       0b001001
#define DRAW_LINED      0b001010
#define DRAW_LINEDI     0b001011
#define DRAW_TEX0       0b001100
#define DRAW_TEX1       0b010011
#define DRAW_TEX2       0b001101
#define DRAW_TEX3       0b001110
#define DRAW_TEX4       0b001111
#define DRAW_TEX5       0b010000
#define DRAW_TEX6       0b010001
#define DRAW_TEX7       0b010010

Node_heap_s pushHeap;

GameLoop_t gameLoop;

void setupConsole();

void registerGameLoop(GameLoop_t);

void drawLoop();

void consoleLogic();

#endif