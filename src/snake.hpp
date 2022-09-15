#pragma once

#include <GL.hpp>
#include <Console-IO.hpp>
#include <sys/random.h>

uint8_t sprites[128] = {0b00111111, 0b11111100, 0b10111111, 0b11110011, 0b00000111, 0b11100000, 0b01111111, 0b11101101, 0b00000001, 0b10000000, 0b01111111, 0b11101110, 0b00000001, 0b10000000, 0b00001111, 0b11110001, 0b00000000, 0b00000000, 0b00000011, 0b11000000, 0b00000000, 0b00000000, 0b00000001, 0b10001100, 0b01110000, 0b00001110, 0b00000000, 0b00010000, 0b00110000, 0b00001100, 0b00000000, 0b00100000, 0b00000000, 0b00000000, 0b00000000, 0b00100000, 0b00000010, 0b01000000, 0b00000000, 0b00000000, 0b00001100, 0b00110000, 0b00000000, 0b00000000, 0b11110000, 0b00001111, 0b00000001, 0b10000000, 0b00000010, 0b01000000, 0b00000001, 0b10000000, 0b00001100, 0b00110000, 0b00000011, 0b11000000, 0b11110000, 0b00001111, 0b00000111, 0b11100000, 0b00000000, 0b00000000, 0b10011111, 0b11111001, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000010, 0b01000000, 0b00000000, 0b00000000, 0b00001100, 0b00110000, 0b00000000, 0b00000000, 0b11110001, 0b10001111, 0b00000000, 0b00000000, 0b00000001, 0b10000000, 0b00000000, 0b00000000, 0b00001001, 0b10010000, 0b00000000, 0b00000000, 0b00110001, 0b10001100, 0b00000000, 0b00000000, 0b11000011, 0b11000011, 0b00000000, 0b00000000, 0b00000011, 0b11000000, 0b00000000, 0b00000000, 0b00010111, 0b11101000, 0b00000000, 0b00000000, 0b01100111, 0b11100110, 0b00000000, 0b00000000, 0b10001111, 0b11110001, 0b00000000, 0b00000000, 0b00001111, 0b11110000, 0b00000000, 0b00000000, 0b10011111, 0b11111001, 0b00000000, 0b00000000, 0b00111111, 0b11111100, 0b00000000, 0b00000000, 0b01111111, 0b11111110 };

#define FIELD_W 23
#define FIELD_H 13

#define UP -1
#define LEFT -1
#define RIGHT 1
#define DOWN 1

#define EMPTY 0
#define SNAKE 1
#define APPLE 2

struct vec2 {
    uint16_t x;
    uint16_t y;
};

vec2 apple;
vec2 snake[FIELD_H * FIELD_W];
uint16_t snake_length = 3;
bool gameOver = false;

int8_t travel_dir_v = 0; // vertical and horizontal travel directions
int8_t travel_dir_h = 0;


void snakeSetup(GL* gfx) 
{
    gfx->loadTileMap(sprites, 32, 32, 16, 16); // load tile map
    gfx->loadTileFromMap(0, 0, TEXTURE_BINDING_0); // save the snake head and tale to textures becouse they will be rotated
    gfx->loadTileFromMap(1, 1, TEXTURE_BINDING_1);

    apple.x = (uint16_t)(esp_random()%23);
    apple.y = (uint16_t)(esp_random()%13);

    snake[0].x = 12;
    snake[0].y = 7;
    snake[1].x = 12;
    snake[1].y = 8;
    snake[2].x = 12;
    snake[2].y = 9;
}

void pickupApple() // call this when the apple gets pickeup
{
    apple.x = (uint16_t)(esp_random()%23);
    apple.y = (uint16_t)(esp_random()%13);
    for(uint16_t i = 0; i < snake_length; i++)
        if(apple.x == snake[i].x && apple.y == snake[i].y)
            pickupApple(); // if the apple is generated inside the snake we want to generate it again
}

void snakeFinish(GL* gfx) 
{ // more or less a clean function
    gfx->clearTexture(TEXTURE_BINDING_0);
    gfx->clearTexture(TEXTURE_BINDING_1);
}

bool checkCollisions() {
    if(snake[0].x + travel_dir_h > 0 && snake[0].x + travel_dir_h <= 0 || snake[0].y + travel_dir_v > 0 && snake[0].y + travel_dir_v <= 0) return true;

    for(uint16_t i = 1; i < snake_length; i++){
        if(snake[0].x + travel_dir_h == snake[i].x || snake[0].y + travel_dir_v == snake[i].y) return true;
    }

    return false;
}

void snakeLoop(GL* gfx) 
{
    if(gameOver) return;
    uint8_t travel_v = (isPressedSticky(BUTTON_UP_ID) * UP) + (isPressedSticky(BUTTON_DOWN_ID) * DOWN);
    uint8_t travel_h = (isPressedSticky(BUTTON_LEFT_ID) * LEFT) + (isPressedSticky(BUTTON_RIGHT_ID) * RIGHT);
    if(travel_v != 0)
    {
        travel_dir_v = travel_v;
        travel_dir_h = 0;
    } else if(travel_h != 0) {
        travel_dir_h = travel_h;
        travel_dir_v = 0;
    }

    gfx->clearDisplayBuffer();

    gfx->fillRect(0, 0, 400, 16, BLACK); // draw black borders
    gfx->fillRect(0, 16, 16, 208, BLACK);
    gfx->fillRect(384, 16, 16, 208, BLACK);
    gfx->fillRect(0, 224, 400, 16, BLACK);
    gfx->printf("Score: %d", snake_length-3);

    gfx->drawTileFromMap(16 + apple.x*16, 16 + apple.y * 16, 1, 0);

    for(uint16_t i = snake_length-1; i > 0; i--)
    {
        gfx->drawTileFromMap(16 + snake[i].x*16, 16 + snake[i].y*16, 0, 1);
        snake[i].x = snake[i-1].x;
        snake[i].y = snake[i-1].y;
    }

    if(snake[0].x == apple.x && snake[0].y == apple.y) 
    {
        pickupApple();
        snake_length++;
    }

    gfx->drawTexture(16 + snake[0].x*16, 16 + snake[0].y*16, TEXTURE_BINDING_0);
    snake[0].x += travel_dir_h;
    snake[0].y += travel_dir_v;

    gameOver = checkCollisions();

    
    gfx->refresh();
}