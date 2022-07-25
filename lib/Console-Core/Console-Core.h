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

#ifndef _CONSOLE_
#define _CONSOLE_

#include <GL.h>
#include <FreeRTOSConfig.h>

#define NONE 0
#define GAME_1 1
#define GAME_2 2
#define GAME_3 3
#define GAME_4 4

#ifndef BUTTON_UP
#define BUTTON_UP 35
#endif

#ifndef BUTTON_DOWN
#define BUTTON_DOWN 33
#endif

#ifndef BUTTON_LEFT
#define BUTTON_LEFT 34
#endif

#ifndef BUTTON_RIGHT
#define BUTTON_RIGHT 32
#endif

#ifndef BUTTON_A
#define BUTTON_A 12
#endif

#ifndef BUTTON_B
#define BUTTON_B 14
#endif

typedef void(*GameSetups_t[5])();

typedef void(*GameLoops_t[5])();

typedef void(*GameSetup_t)();

typedef void(*GameLoop_t)();

uint8_t numGames = 1;

uint16_t screenWidth, screenHeight;

GameLoops_t gameLoops;

GameSetups_t gameSetups;

char* gameNames[4];

// graphics library
GL* gl;

/**
 * @brief sets up the game console interface etc...
 * 
 * @param clk pin of the display
 * 
 * @param di pin of the display
 * 
 * @param cs pin of the display
 * 
 * @param screenWidth width of the screen that is attached
 * 
 * @param screenHeight height of the screen that is attached
*/
void setupConsole(uint8_t clk, uint8_t di, uint8_t cs, uint16_t screenWidth, uint16_t screenHeight);

/**
 * @brief gets the graphics library
*/
GL* getGraphicsLibrary(void)
{
    return gl;
}

/**
 * @brief registers a game loop
 * 
 * @param gameSetup function that runs before the actual game launches for setup purpouses
 * 
 * @param game game loop that will run once the game is started
 * 
 * @param gameName display name of the game
*/
void registerGameLoop(GameSetup_t gameSetup, GameLoop_t game, const char* gameName)
{
    if(numGames < 5)
    {
        gameSetups[numGames] = gameSetup;
        gameLoops[numGames] = game;
        gameNames[numGames] = (char*)gameName;
        numGames++;
    }
}

/**
 * @brief does setup for the console interface
*/
void consoleInterfaceSetup()
{

}

/**
 * @brief logic of the game console should run every loop
*/
void consoleInterface()
{
    
}

/**
 * @brief launches game at game ID
 * 
 * @param gameId launches game at gameId slot (default is GAME_1)
*/
void launchGame(uint8_t gameId)
{

}

void setupConsole(uint8_t clk, uint8_t di, uint8_t cs, uint16_t screenWidth, uint16_t screenHeight)
{

}

#endif