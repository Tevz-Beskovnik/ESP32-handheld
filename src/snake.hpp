#pragma once

#include <GL.hpp>
#include <Console-Core.hpp>
#include <Console-IO.hpp>

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
bool game_over = false;

int8_t travel_dir_v = 0; // vertical and horizontal travel directions
int8_t travel_dir_h = 0;

uint8_t head_rotation = TEXTURE_BINDING_0;

uint32_t refresh_counter = 0;

void snakeSetup(GL* gfx)
{
    gfx->loadTileMap(sprites, 32, 32, 16, 16); // load tile map
    gfx->loadTileFromMap(0, 0, TEXTURE_BINDING_0); // save the snake head to textures becouse they will be rotated
    gfx->loadTileFromMap(0, 0, TEXTURE_BINDING_1);
    gfx->rotateTexture(TEXTURE_BINDING_1, ROTATE_90);
    gfx->loadTileFromMap(0, 0, TEXTURE_BINDING_2);
    gfx->rotateTexture(TEXTURE_BINDING_2, ROTATE_180);
    gfx->loadTileFromMap(0, 0, TEXTURE_BINDING_3);
    gfx->rotateTexture(TEXTURE_BINDING_3, ROTATE_270);

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

    game_over = false;
    snake_length = 3;
    travel_dir_h = 0;
    travel_dir_v = 0;
    refresh_counter = 0;
}

// check if the snake is touching the perimiter of is bumping into itself
bool checkCollisions() {
    // this is cheking the snakes head if its touching the perimiter
    if(((int)(snake[0].x + travel_dir_h) < 0 || (int)(snake[0].x + travel_dir_h) > FIELD_W-1) || ((int)(snake[0].y + travel_dir_v) < 0 || (int)(snake[0].y + travel_dir_v) > FIELD_H-1)) return true;

    // this is checking if the snake is touching itself anywhere
    for(uint16_t i = 1; i < snake_length; i++){
        if(snake[0].x + travel_dir_h == snake[i].x && snake[0].y + travel_dir_v == snake[i].y) return true;
    }

    return false;
}

void snakeLoop(GL* gfx) 
{
    if(game_over) exitGame();
    if(isPressed(BUTTON_DOWN_ID) && travel_dir_v != UP){
        head_rotation = TEXTURE_BINDING_2;
        travel_dir_v = DOWN;
        travel_dir_h = 0;
    }
    if(isPressed(BUTTON_UP_ID) && travel_dir_v != DOWN){
        head_rotation = TEXTURE_BINDING_0;
        travel_dir_v = UP;
        travel_dir_h = 0;
    }
    if(isPressed(BUTTON_LEFT_ID) && travel_dir_h != RIGHT){
        head_rotation = TEXTURE_BINDING_1;
        travel_dir_v = 0;
        travel_dir_h = LEFT;
    }
    if(isPressed(BUTTON_RIGHT_ID) && travel_dir_h != LEFT){
        head_rotation = TEXTURE_BINDING_3;
        travel_dir_v = 0;
        travel_dir_h = RIGHT;
    }



    if(refresh_counter == 15000) {
        refresh_counter = 0;
        gfx->clearDisplayBuffer();

        gfx->fillRect(0, 0, 400, 16, BLACK); // draw black borders
        gfx->fillRect(0, 16, 16, 208, BLACK);
        gfx->fillRect(384, 16, 16, 208, BLACK);
        gfx->fillRect(0, 224, 400, 16, BLACK);
        gfx->setCursor(16, 0);
        gfx->textColor(WHITE);
        gfx->printf("Score: %d", snake_length-3);

        gfx->drawTileFromMap(16 + apple.x*16, 16 + apple.y * 16, 1, 0);

        for(uint16_t i = snake_length-1; i > 0; i--)
        {
            gfx->drawTileFromMap(16 + snake[i].x*16, 16 + snake[i].y*16, 0, 1);
            if(travel_dir_h != 0 || travel_dir_v != 0) {
                snake[i].x = snake[i-1].x;
                snake[i].y = snake[i-1].y;
            }
        }

        if(snake[0].x == apple.x && snake[0].y == apple.y) 
        {
            pickupApple();
            snake_length++;
        }

        game_over = checkCollisions();

        gfx->drawTexture(16 + snake[0].x*16, 16 + snake[0].y*16, head_rotation);
        snake[0].x += travel_dir_h;
        snake[0].y += travel_dir_v;

        gfx->refresh();
    }
    refresh_counter++;
}