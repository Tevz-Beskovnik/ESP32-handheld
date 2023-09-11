#include <Wire.h>
#include <WiFi.h>
#include <Arduino.h>
#include <Console-Core.hpp>
#include <deauther.hpp>
#include <game1.hpp>
#include <game2.hpp>
#include <snake.hpp>
#include <Active-Object.h>

#define SHARP_CLK 25
#define SHARP_DI 26
#define SHARP_CS 27

Console console(SHARP_CLK, SHARP_DI, SHARP_CS, 400, 240);

Active object;

void setup(void)
{
  Serial.begin(9600);
  delay(2000);
  Serial.println("Hello!");

  object.dispatcher = graphics_dispatcher;
  object.params = console.get_gfx();
  object.task_name = "Graphics";
  object.queue = xQueueCreate(20, sizeof(Event));

  active_object_start(&object);

  console.set_active_object(&object);

  console.add_application(new Game1());
  console.add_application(new Deauther());
  console.add_application(new Snake());

  Serial.println("Texture loaded");
  Serial.printf("Flash size: %i\n", ESP.getFlashChipSize());
}

void loop(void)
{
  console.loop();
}