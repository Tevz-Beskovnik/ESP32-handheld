#include <Wire.h>
#include <Arduino.h>
#include <Console-Core.hpp>

#define SHARP_CLK  25
#define SHARP_DI 26
#define SHARP_CS   27

#define BLACK 0
#define WHITE 1

#define MOVEMENT_SPEED 8

uint16_t x = 200, y = 0;
/*uint8_t sprite[128] = {
  0b11111111, 0b11111111, 0b11111111, 0b11111111, 
  0b11111111, 0b10111111, 0b11111111, 0b11111111, 
  0b11111111, 0b01011111, 0b11111110, 0b11111111, 
  0b11111111, 0b00001111, 0b11111000, 0b11111111, 
  0b11111111, 0b00000111, 0b11100110, 0b11111111, 
  0b11111111, 0b00000011, 0b11010000, 0b11111111, 
  0b11111111, 0b00000001, 0b10100000, 0b11111111, 
  0b11111111, 0b01110000, 0b01001110, 0b11111111, 
  0b11111111, 0b11111000, 0b00011111, 0b11111111, 
  0b01111111, 0b11111110, 0b01111111, 0b11111110, 
  0b10111111, 0b11111110, 0b01111111, 0b11111101, 
  0b10111111, 0b10011110, 0b01111001, 0b11111101, 
  0b10111111, 0b10111111, 0b11111101, 0b11111101, 
  0b01111111, 0b11111111, 0b11111111, 0b11111110, 
  0b11111111, 0b11111110, 0b01111111, 0b11111111, 
  0b11111111, 0b11111110, 0b01111111, 0b11111111, 
  0b11111111, 0b00111110, 0b01111100, 0b11111111, 
  0b11111111, 0b11111101, 0b10111111, 0b11111111, 
  0b11111111, 0b11111011, 0b11011111, 0b11111111, 
  0b11111111, 0b11100111, 0b11100111, 0b11111111, 
  0b11111111, 0b11011000, 0b00011011, 0b11111111, 
  0b01111111, 0b00111111, 0b11111100, 0b11111110, 
  0b01111111, 0b11111011, 0b11011110, 0b11111110, 
  0b01111111, 0b11111010, 0b01011111, 0b11111110, 
  0b01111111, 0b11111001, 0b10011110, 0b11111110, 
  0b01111111, 0b11111011, 0b11011111, 0b11111110, 
  0b11111111, 0b00000000, 0b00000000, 0b11111111, 
  0b11111111, 0b01111011, 0b11011110, 0b11111111, 
  0b11111111, 0b01111011, 0b11011110, 0b11111111, 
  0b11111111, 0b01100011, 0b11000110, 0b11111111, 
  0b11111111, 0b00011011, 0b11011000, 0b11111111, 
  0b11111111, 0b10000011, 0b11000001, 0b11111111, 
};

uint8_t house[128] = {
  0b11111111, 0b00111111, 0b11111100, 0b11111111,
  0b11111111, 0b10011111, 0b11111001, 0b11111111,
  0b11111111, 0b10101111, 0b11110101, 0b11111111,
  0b11111111, 0b00110111, 0b11101100, 0b11111111, 
  0b11111111, 0b10111011, 0b11011001, 0b11111111, 
  0b11111111, 0b10111001, 0b10010101, 0b11111111, 
  0b11111111, 0b10011010, 0b01001101, 0b11111111, 
  0b01111111, 0b00101011, 0b10011100, 0b11111110, 
  0b10111111, 0b10110001, 0b01011001, 0b11111101, 
  0b10011111, 0b10111010, 0b10010101, 0b11111000, 
  0b00101111, 0b00111011, 0b01001100, 0b11110101, 
  0b10110111, 0b11011011, 0b10011011, 0b11101000, 
  0b10111011, 0b11101001, 0b01010111, 0b11010101, 
  0b10111011, 0b00110010, 0b10001100, 0b11001000, 
  0b00011011, 0b11011011, 0b01011011, 0b11010101, 
  0b10101011, 0b11011101, 0b10111011, 0b11001000, 
  0b10110011, 0b11011110, 0b01111011, 0b11010101, 
  0b00111011, 0b11011111, 0b11111011, 0b11001000, 
  0b10011011, 0b00111111, 0b11111100, 0b11010101, 
  0b10101011, 0b11111111, 0b11111111, 0b11001001, 
  0b10110111, 0b11111111, 0b11111111, 0b11010101, 
  0b10111111, 0b11100001, 0b10000011, 0b11101101, 
  0b10111111, 0b11011110, 0b01111101, 0b11111101, 
  0b10111111, 0b11011110, 0b01111101, 0b11111101, 
  0b10111111, 0b11011110, 0b01111101, 0b11111101, 
  0b10111111, 0b11011110, 0b01111101, 0b11111101, 
  0b10111111, 0b11011110, 0b01111001, 0b11111101, 
  0b10111111, 0b11011110, 0b01111101, 0b11111101, 
  0b10111111, 0b11000000, 0b01111101, 0b11111101, 
  0b10111111, 0b11111111, 0b00000001, 0b11111101, 
  0b10111111, 0b11111111, 0b01000000, 0b11111101, 
  0b01111111, 0b00000000, 0b00000000, 0b11111110 
}; */

uint8_t tileMap[512] = {
  0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b01111111, 0b11111110, 0b11111111, 
  0b11111111, 0b10111111, 0b11111111, 0b11111111, 0b11111111, 0b10111111, 0b11111100, 0b11111111, 
  0b11111111, 0b01011111, 0b11111110, 0b11111111, 0b11111111, 0b10011111, 0b11111010, 0b11111111,
  0b11111111, 0b00001111, 0b11111000, 0b11111111, 0b11111111, 0b10101111, 0b11110010, 0b11111111, 
  0b11111111, 0b00000111, 0b11100110, 0b11111111, 0b11111111, 0b11100111, 0b11101000, 0b11111111, 
  0b11111111, 0b00000011, 0b11010000, 0b11111111, 0b11111111, 0b10101011, 0b11001010, 0b11111111, 
  0b11111111, 0b00000001, 0b10100000, 0b11111111, 0b11111111, 0b10111001, 0b10100010, 0b11111111, 
  0b11111111, 0b01110000, 0b01001110, 0b11111111, 0b11111111, 0b11101010, 0b00101000, 0b11111111, 
  0b11111111, 0b11111000, 0b00011111, 0b11111111, 0b01111111, 0b10101110, 0b10001010, 0b11111110, 
  0b01111111, 0b11111110, 0b01111111, 0b11111110, 0b10111111, 0b00111010, 0b10100010, 0b11111100, 
  0b10111111, 0b11111110, 0b01111111, 0b11111101, 0b10011111, 0b10101011, 0b00101000, 0b11111010, 
  0b10111111, 0b10011110, 0b01111001, 0b11111101, 0b10101111, 0b11001110, 0b10001010, 0b11110010, 
  0b10111111, 0b10111111, 0b11111101, 0b11111101, 0b11100111, 0b11001010, 0b10100110, 0b11101000, 
  0b01111111, 0b11111111, 0b11111111, 0b11111110, 0b10101011, 0b11010011, 0b00100110, 0b11101010, 
  0b11111111, 0b11111110, 0b01111111, 0b11111111, 0b10111011, 0b11011010, 0b10010110, 0b11100010, 
  0b11111111, 0b11111110, 0b01111111, 0b11111111, 0b11101011, 0b11011000, 0b10110110, 0b11101000, 
  0b11111111, 0b00111110, 0b01111100, 0b11111111, 0b10101011, 0b11011010, 0b00110110, 0b11101010, 
  0b11111111, 0b11111101, 0b10111111, 0b11111111, 0b00111011, 0b11011011, 0b10110110, 0b11100010, 
  0b11111111, 0b11111011, 0b11011111, 0b11111111, 0b10101011, 0b11011011, 0b10110110, 0b11101001, 
  0b11111111, 0b11100111, 0b11100111, 0b11111111, 0b00001011, 0b00000000, 0b00000000, 0b11101000, 
  0b11111111, 0b11011000, 0b00011011, 0b11111111, 0b00101011, 0b11111111, 0b11111111, 0b11110100, 
  0b01111111, 0b00111111, 0b11111100, 0b11111110, 0b10110111, 0b11111111, 0b11111111, 0b11111101, 
  0b01111111, 0b11111011, 0b11011110, 0b11111110, 0b10111111, 0b11111111, 0b11111111, 0b11111101, 
  0b01111111, 0b11111010, 0b10011111, 0b11111110, 0b10111111, 0b11100001, 0b11000011, 0b11111101, 
  0b01111111, 0b11111001, 0b01011110, 0b11111110, 0b10111111, 0b11011110, 0b10111101, 0b11111101, 
  0b01111111, 0b11111011, 0b11011111, 0b11111110, 0b10111111, 0b11011110, 0b10111101, 0b11111101, 
  0b11111111, 0b00000000, 0b00000000, 0b11111111, 0b10111111, 0b11011110, 0b10111101, 0b11111101, 
  0b11111111, 0b01111011, 0b11011110, 0b11111111, 0b10111111, 0b11011110, 0b11000011, 0b11111101, 
  0b11111111, 0b01111011, 0b11011110, 0b11111111, 0b10111111, 0b11011100, 0b10000001, 0b11111101, 
  0b11111111, 0b01100011, 0b11000110, 0b11111111, 0b10111111, 0b11011110, 0b11111111, 0b11111101, 
  0b11111111, 0b00011011, 0b11011000, 0b11111111, 0b00111111, 0b11011110, 0b11111111, 0b11111100, 
  0b11111111, 0b10000011, 0b11000001, 0b11111111, 0b00111111, 0b00000000, 0b00000000, 0b11111100, 
  0b01111111, 0b11111110, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 
  0b10111111, 0b00111101, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 
  0b11011111, 0b11011011, 0b11111110, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 
  0b11101111, 0b11100111, 0b11111101, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 
  0b11110111, 0b11110111, 0b11111011, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 
  0b11111011, 0b11011011, 0b11110111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 
  0b11111101, 0b10111101, 0b11101111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 
  0b11111110, 0b01111110, 0b11011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 
  0b01111110, 0b11111111, 0b10111110, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 
  0b10111101, 0b11111101, 0b00111101, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 
  0b11011011, 0b11111011, 0b11011011, 0b11111110, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 
  0b11100111, 0b11110111, 0b11101111, 0b11111101, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 
  0b11110111, 0b11101111, 0b11110111, 0b11111011, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 
  0b11011011, 0b11011111, 0b11011011, 0b11110111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 
  0b10111101, 0b10111111, 0b10111101, 0b11101111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 
  0b01111101, 0b11111111, 0b01111110, 0b11011111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 
  0b11111011, 0b01111110, 0b11111111, 0b10111110, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 
  0b11110111, 0b10111101, 0b11111101, 0b10111101, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 
  0b11101111, 0b11011011, 0b11111011, 0b11011011, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 
  0b11011111, 0b11101111, 0b11110111, 0b11101111, 0b11111111, 0b00011111, 0b11111000, 0b11111111, 
  0b10111111, 0b11110111, 0b11101111, 0b11100111, 0b11111111, 0b11100111, 0b11100111, 0b11111111, 
  0b01111111, 0b11011011, 0b11011111, 0b11011011, 0b11111111, 0b11111011, 0b11011111, 0b11111111, 
  0b11111111, 0b10111100, 0b10111111, 0b10111101, 0b11111111, 0b11111101, 0b10111111, 0b11111111, 
  0b11111111, 0b01111101, 0b11111111, 0b01111110, 0b11111111, 0b11111110, 0b01111111, 0b11111111, 
  0b11111111, 0b11111011, 0b01111110, 0b01111111, 0b11111111, 0b11010110, 0b01111111, 0b11111111, 
  0b11111111, 0b11110111, 0b10111101, 0b10111111, 0b01111111, 0b11111111, 0b11111111, 0b11111110, 
  0b11111111, 0b11101111, 0b11011011, 0b11011111, 0b01111111, 0b10111011, 0b11111111, 0b11111110, 
  0b11111111, 0b11011111, 0b11101111, 0b11101111, 0b01111111, 0b11000111, 0b11111111, 0b11111110, 
  0b11111111, 0b10111111, 0b11100111, 0b11110111, 0b01111111, 0b11111111, 0b11111111, 0b11111110, 
  0b11111111, 0b01111111, 0b11011011, 0b11111011, 0b01111111, 0b11111111, 0b11111111, 0b11111110, 
  0b11111111, 0b11111111, 0b10111100, 0b11111101, 0b11111111, 0b11111110, 0b01111111, 0b11111111, 
  0b11111111, 0b11111111, 0b01111111, 0b11111110, 0b11111111, 0b00000001, 0b10000000, 0b11111111 
};

void gameFinish1()
{
  Serial.println("end");
}

void gameSetup1() 
{
  Serial.println("Hello Amerika!");
  gl->loadTileMap(tileMap, 64, 64, 32, 32);
  gl->loadTileFromMap(0, 0, TEXTURE_BINDING_0);
  gl->loadTileFromMap(1, 0, TEXTURE_BINDING_1);
}

void gameLoop1()
{
  gl->clearDisplayBuffer();
  gl->drawTexture(192, 64, TEXTURE_BINDING_1);
  gl->drawTexture(64, 32, TEXTURE_BINDING_1);
  gl->drawTexture(64, 160, TEXTURE_BINDING_1);
  gl->drawTexture(128, 128, TEXTURE_BINDING_1);
  gl->drawTexture(320, 192, TEXTURE_BINDING_1);
  if(digitalRead(BUTTON_UP))
    y-=MOVEMENT_SPEED;
  if(digitalRead(BUTTON_DOWN))
    y+=MOVEMENT_SPEED;
  if(digitalRead(BUTTON_LEFT))
    x-=MOVEMENT_SPEED;
  if(digitalRead(BUTTON_RIGHT))
    x+=MOVEMENT_SPEED;
  //display.drawBitmap(x, y, sprite, 32, 32, BLACK);
  //display.fillRectD(x, y, 40, 40);
  gl->drawTexture(x, y, TEXTURE_BINDING_0);
  gl->refresh();
}

void setup(void)
{
  Serial.begin(9600);
  delay(2000);
  Serial.println("Hello!");
  registerGame(gameSetup1, gameLoop1, gameFinish1,  "Test game 1");
  registerGame(gameSetup1, gameLoop1, gameFinish1, "Test game 2");
  registerGame(gameSetup1, gameLoop1, gameFinish1, "Crash bandicoot");
  registerGame(gameSetup1, gameLoop1, gameFinish1, "Destiny 2");

  setupConsole(SHARP_CLK, SHARP_DI, SHARP_CS, 400, 240);

  //display.loadTexture(sprite, 32, 32, TEXTURE_BINDING_0);
  //display.loadTexture(house, 32, 32, TEXTURE_BINDING_1);
  /*display.loadTileMap(tileMap, 64, 64, 32, 32);
  display.loadTileFromMap(0, 0, TEXTURE_BINDING_0);
  display.loadTileFromMap(1, 0, TEXTURE_BINDING_1);*/
  Serial.println("Texture loaded");
  Serial.printf("Flash size: %i\n", ESP.getFlashChipSize());
  gl->clearDisplayBuffer();
  gl->fillRect(0, 0, 400, 240, BLACK);
  for(uint16_t i = 1; i < 50; i++)
    gl->drawFastRawHLine(2, i, i, WHITE);

  for(uint16_t i = 51; i < 102; i++)
    gl->drawLine(2, i, 2+i-51, i, WHITE);
  gl->refresh();
}


void loop(void)
{
  gl->refresh();
  //consoleLoop();
}