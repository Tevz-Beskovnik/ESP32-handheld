#include <Wire.h>
#include <Arduino.h>
#include <Console-Core.hpp>
#include <game1.hpp>
#include <game2.hpp>

#define SHARP_CLK  25
#define SHARP_DI 26
#define SHARP_CS   27

#define BLACK 0
#define WHITE 1

void setup(void)
{
  Serial.begin(9600);
  delay(2000);
  Serial.println("Hello!");
  registerGame(gameSetup1, gameLoop1, gameFinish1,  "Test game 1");
  registerGame(gameSetup2, gameLoop2, gameFinish2, "Some maze");

  setupConsole(SHARP_CLK, SHARP_DI, SHARP_CS, 400, 240);

  //display.loadTexture(sprite, 32, 32, TEXTURE_BINDING_0);
  //display.loadTexture(house, 32, 32, TEXTURE_BINDING_1);
  /*display.loadTileMap(tileMap, 64, 64, 32, 32);
  display.loadTileFromMap(0, 0, TEXTURE_BINDING_0);
  display.loadTileFromMap(1, 0, TEXTURE_BINDING_1);*/
  Serial.println("Texture loaded");
  Serial.printf("Flash size: %i\n", ESP.getFlashChipSize());
}


void loop(void)
{
  consoleLoop();
}