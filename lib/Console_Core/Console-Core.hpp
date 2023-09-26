/**
 * @author: Tevž Beškovnik
 * @date: 23. 7. 2022
 * @description: handles everything to do with logic of game console, drawing, game loop ect....
 */

/*
TODO:
    - Implement OTA for wireless newGameInclusion
    - Add boot screen when console launches
    - Flash make the console UI
    - Think of implementing functions that run when game launch (for setup and stuff)
    - Add external flash (????) - this one is in the air
*/
#pragma once

#include <GL.hpp>
#include <Console-IO.hpp>
#include <FreeRTOSConfig.h>
#include <SPI_driver.hpp>
#include <Keyboard-Component.hpp>
#include <Application.hpp>
#include <Console-UI.hpp>
#include <Active-Object.hpp>

#define CONSOLE_INTERFACE 0
#define GAME_1 1
#define GAME_2 2
#define GAME_3 3
#define GAME_4 4

class Console
{
public:
    Console(uint8_t clk, uint8_t di, uint8_t cs, uint16_t screenWidth, uint16_t screenHeight);

    ~Console();

    void setup();

    void loop();

    void set_active_object(Active *object);

    bool add_application(Application *app);

    GL *get_gfx() const;

private:
    Application *apps[5];

    uint8_t previous_active_game;
    uint8_t active_game;

    uint8_t number_of_games;

    uint16_t screen_width, screen_height;

    uint8_t *contextBuffer;

    GL *gfx;

    Keyboard *kb;

    ConsoleUI *console_UI;

    SPIDriver *spi_driver;
};