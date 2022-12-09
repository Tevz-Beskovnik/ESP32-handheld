#pragma once

#include <Console-IO.hpp>
#include <GL.hpp>

// screen res 400 240
#define GAME_W 19
#define GAME_H 13

uint8_t tilemap2[288] = {0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b00000000, 0b00000000, 0b00111111, 0b11111110, 0b11111111, 0b11111111, 0b00000000, 0b00000000, 0b00011111, 0b11111101, 0b11111111, 0b11111111, 0b00000000, 0b00000000, 0b00111111, 0b11111010, 0b11111111, 0b11111111, 0b00000000, 0b00000000, 0b00001111, 0b11100010, 0b11111111, 0b11111111, 0b00000000, 0b00000000, 0b00110111, 0b11011010, 0b11111111, 0b11111111, 0b00000000, 0b00000000, 0b11001111, 0b11100111, 0b11111111, 0b11111111, 0b00000000, 0b00000000, 0b00001111, 0b11111000, 0b01111111, 0b11111000, 0b00000000, 0b00000000, 0b10001111, 0b11111010, 0b00011111, 0b11100000, 0b00000000, 0b00000000, 0b00011111, 0b11111000, 0b00001111, 0b11000000, 0b00000000, 0b00000000, 0b11111111, 0b11111101, 0b00001111, 0b11000000, 0b00000000, 0b00000000, 0b00011111, 0b11111101, 0b10001111, 0b11100000, 0b00000000, 0b00000000, 0b00001111, 0b11111010, 0b10001111, 0b11100100, 0b00000000, 0b00000000, 0b00001111, 0b11111010, 0b00010011, 0b11000000, 0b00000000, 0b00000000, 0b00001111, 0b11111000, 0b11100011, 0b11010101, 0b00000000, 0b00000000, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b11111111, 0b11111111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b11111111, 0b11111111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b11111111, 0b11111111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b11111111, 0b11111111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b11111111, 0b11111111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b11111111, 0b11111111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b11111111, 0b11111111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00011111, 0b11111000, 0b00000000, 0b00000000, 0b00101110, 0b11101100, 0b00100111, 0b11100000, 0b11101110, 0b11101110, 0b11000000, 0b00000011, 0b01000011, 0b11000000, 0b00000000, 0b00000000, 0b01101011, 0b10110110, 0b11000011, 0b11111111, 0b10111011, 0b10111011, 0b01100000, 0b00000110, 0b01000011, 0b11000100, 0b00000000, 0b00000000, 0b10101110, 0b11100111, 0b01000011, 0b11000100, 0b11101110, 0b11101110, 0b11100000, 0b00000111, 0b01000011, 0b11000000, 0b00000000, 0b00000000, 0b11101011, 0b10110111, 0b01000011, 0b11000000, 0b10111011, 0b10111011, 0b00000000, 0b00000000, 0b11111111, 0b11111111, 0b00000000, 0b00000000, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b10111111, 0b11111010, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b00011111, 0b11111000, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b00011111, 0b11111000, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b10011111, 0b11111010, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b00011111, 0b11111100, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b10011111, 0b11100100, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b10011111, 0b11100100, 0b11111111, 0b11100011, 0b11111111, 0b11111111, 0b11111111, 0b11111100, 0b11111111, 0b11011101, 0b11111111, 0b11111111, 0b00011111, 0b11111100, 0b00000011, 0b11011100, 0b11111111, 0b11111111, 0b00011111, 0b11111100, 0b11111111, 0b11011101, 0b11111111, 0b11111111, 0b10011111, 0b11111100, 0b11010111, 0b11100011, 0b11111111, 0b11111111, 0b10011111, 0b11111100, 0b11011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111 };

uint8_t game_x = 1;
uint8_t game_y = 1;

uint8_t game_map[GAME_H][GAME_W] = {
    {1, 4, 1, 4, 4, 4, 4, 1, 4, 4, 1, 4, 1, 1, 1, 4, 1, 1, 1},
    {1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 4, 4, 4, 0, 4, 4, 1},
    {1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 8, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 1, 0, 1, 4, 0, 4, 4, 4, 4, 4, 4, 1, 0, 1},
    {1, 0, 4, 4, 0, 4, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 4, 4, 4, 4, 1, 0, 1, 0, 1},
    {1, 0, 4, 4, 1, 4, 4, 4, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 4, 0, 1, 0, 1},
    {1, 4, 4, 0, 1, 0, 1, 1, 1, 4, 0, 1, 0, 1, 0, 8, 1, 0, 1},
    {1, 5, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 4, 4, 4, 1, 0, 1},
    {1, 1, 1, 0, 4, 0, 4, 6, 4, 0, 1, 1, 0, 0, 0, 0, 4, 0, 1},
    {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 8, 1, 1, 0, 0, 7, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

void gameFinish2(GL* gfx)
{
  gfx->clearTexture(TEXTURE_BINDING_0);
  gfx->clearTexture(TEXTURE_BINDING_1);
  gfx->clearTexture(TEXTURE_BINDING_2);
  gfx->clearTexture(TEXTURE_BINDING_3);
  gfx->clearTexture(TEXTURE_BINDING_4);
  gfx->clearTexture(TEXTURE_BINDING_5);
  gfx->clearTexture(TEXTURE_BINDING_6);
  gfx->clearTexture(TEXTURE_BINDING_7);
}

void gameSetup2(GL* gfx) 
{
  gfx->loadTileMap(tilemap2, 48, 48, 16, 16);
  gfx->loadTileFromMap(2, 0, TEXTURE_BINDING_0);
  gfx->loadTileFromMap(1, 0, TEXTURE_BINDING_1);
  gfx->loadTileFromMap(0, 0, TEXTURE_BINDING_2);
  gfx->loadTileFromMap(2, 1, TEXTURE_BINDING_3);
  gfx->loadTileFromMap(1, 1, TEXTURE_BINDING_4);
  gfx->loadTileFromMap(0, 1, TEXTURE_BINDING_5);
  gfx->loadTileFromMap(2, 2, TEXTURE_BINDING_6);
  gfx->loadTileFromMap(1, 2, TEXTURE_BINDING_7);
}

void gameLoop2(GL* gfx)
{
    gfx->clearDisplayBuffer();
    gfx->fillRect(0, 0, 400, 16, BLACK);
    gfx->fillRect(0, 224, 400, 16, BLACK);
    gfx->fillRect(0, 0, 16, 240, BLACK);
    gfx->fillRect(304, 0, 96, 240, BLACK);
    gfx->setCursor(32, 2);
    gfx->textColor(WHITE);
    gfx->print("Score: 0");
    for(int i = 0; i < GAME_H; i++)
    {
        for(int j = 0; j < GAME_W; j++)
        {
            if(game_map[i][j] != 0)
                gfx->drawTexture(j*16, 16 + i*16, game_map[i][j]-1);
        }
    }
    if(is_pressed_sticky(BUTTON_DOWN_ID) && (game_map[game_y+1][game_x] == 0))
        game_y++;
    if(is_pressed_sticky(BUTTON_UP_ID) && (game_map[game_y-1][game_x] == 0))
        game_y--;
    if(is_pressed_sticky(BUTTON_RIGHT_ID) && (game_map[game_y][game_x+1] == 0))
        game_x++;
    if(is_pressed_sticky(BUTTON_LEFT_ID) && (game_map[game_y][game_x-1] == 0))
        game_x--;

    gfx->drawTexture(game_x*16, 16 + game_y*16, TEXTURE_BINDING_2);
    gfx->refresh();
}
