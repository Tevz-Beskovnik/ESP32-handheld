#include <Wire.h>
#include <WiFi.h>
#include <Arduino.h>
#include <Console-Core.hpp>
#include <game1.hpp>
#include <game2.hpp>
#include <snake.hpp>

#define SHARP_CLK 25
#define SHARP_DI 26
#define SHARP_CS 27

Console console(SHARP_CLK, SHARP_DI, SHARP_CS, 400, 240);

void setup(void)
{
  Serial.begin(9600);
  delay(2000);
  Serial.println("Hello!");

  console.add_application(new Game1());
  console.add_application(new Game2());
  console.add_application(new Snake());

  Serial.println("Texture loaded");
  Serial.printf("Flash size: %i\n", ESP.getFlashChipSize());
}

void loop(void)
{
  console.loop();
}