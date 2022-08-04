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

#include <GL.hpp>
#include <textures.h>
#include <Console-IO.hpp>
#include <FreeRTOSConfig.h>

#define CONSOLE_INTERFACE 0
#define GAME_1 1
#define GAME_2 2
#define GAME_3 3
#define GAME_4 4

typedef void(*GameSetups_t[5])();

typedef void(*GameLoops_t[5])();

typedef void(*GameFinishs_t[5])();

typedef void(*GameSetup_t)();

typedef void(*GameLoop_t)();

typedef void(*GameFinish_t)();

uint8_t* UITexture;

uint8_t* contextBuffer;

uint8_t numGames = 1;

uint8_t activeGame = 0;

uint16_t screenWidth, screenHeight;

GameLoops_t gameLoops;

GameSetups_t gameSetups;

GameFinishs_t gameFinishes;

char* gameNames[4];

// UI variables
uint8_t selectedGame = 0;

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
 * @brief registers a game loop
 * 
 * @param gameSetup function that runs before the actual game launches for setup purpouses
 * 
 * @param game game loop that will run once the game is started
 * 
 * @param gameName display name of the game
*/
void registerGame(GameSetup_t gameSetup, GameLoop_t game, GameFinish_t gameFinish, const char* gameName)
{
    if(numGames < 5)
    {
        gameSetups[numGames] = gameSetup;
        gameLoops[numGames] = game;
        gameFinishes[numGames] = gameFinish;
        gameNames[numGames-1] = (char*)gameName;
        numGames++;
    }
}

/**
 * @brief does setup for the console interface
*/
void consoleInterfaceSetup()
{
    gl->loadTexture(controlsSprite, 96, 96, TEXTURE_BINDING_0);

    gl->clearDisplayBuffer();

    gl->fillRect(0, 0, 400, 240, WHITE);
    gl->fillRectD(4, 4, 390, 230);
    gl->fillRect(9, 9, 380, 220, BLACK);
    gl->fillRectD(4, 4, 110, 20);
    gl->fillRect(0, 0, 110, 20, WHITE);
    gl->fillTriangleD(99, 23, 114, 24, 114, 9);
    gl->fillTriangle(104, 24, 114, 25, 115, 14, BLACK);
    gl->setCursor(20, 8);
    gl->fontSize(1);
    gl->textColor(BLACK);
    gl->print("ESP-HANDHEALD");

    /*gl->fillRect(19, 34, 120, 186, WHITE);
    gl->setCursor(28, 40);
    gl->fontSize(2);
    gl->print("Controls:");
    gl->drawTexture(25, 60, TEXTURE_BINDING_0);
    gl->setCursor(22, 140);
    gl->println("Arrow key:");
    gl->setCursor(25, 156);
    gl->println("Selection");
    gl->setCursor(22, 178);
    gl->println("A button:");
    gl->setCursor(25, 194);
    gl->print("Confirm");

    gl->setCursor(150, 35);
    gl->textColor(WHITE);
    gl->print("Games:");*/

    memcpy((void*)UITexture, (void*)contextBuffer, screenHeight*screenWidth/8);

    gl->loadTexture(UITexture, screenWidth, screenHeight, TEXTURE_BINDING_1, true);
}

/**
 * @brief finish function for the console interface
*/
void consoleInterfaceFinish()
{
    gl->clearTexture(TEXTURE_BINDING_0);
    gl->clearTexture(TEXTURE_BINDING_1);
}

/***/
void gameButton(uint8_t color, const char* text, uint8_t i)
{
    gl->fillRect(149, 59+(i*25), (strlen(text)*12)+10, 20, !color);
    gl->fillRect(151, 65+(i*25), 6, 6, color);
    gl->setCursor(160, 61+(i*25));
    gl->fontSize(FONT_SIZE_2);
    gl->textColor(color);
    gl->printf(text);
}

/**
 * @brief launches game at game ID
 * 
 * @param gameId launches game at gameId slot (default is GAME_1)
*/
void launchGame(uint8_t gameId)
{
    gameFinishes[activeGame]();

    activeGame = gameId;

    gameSetups[activeGame]();
}

/**
 * @brief logic of the game console should run every loop
*/
void consoleInterface()
{
    gl->clearDisplayBuffer();

    if(selectedGame > 0 && isPressedSticky(BUTTON_UP_ID))
    {
        selectedGame--;
    }

    if(selectedGame < numGames-2 && isPressedSticky(BUTTON_DOWN_ID)) 
    {
        selectedGame++;
    }

    if(isPressedSticky(BUTTON_A_ID)) launchGame(selectedGame+1);

    gl->drawTexture(0, 0, TEXTURE_BINDING_1);

    for(uint8_t i = 0; i < numGames-1; i++){
        gameButton(selectedGame == i ? BLACK : WHITE, (const char*)gameNames[i], i);
    }

    gl->refresh();
}

/**
 * @brief exits current game
*/
void exitGame() {
    activeGame = CONSOLE_INTERFACE;

    gameSetups[activeGame]();
}

/**
 * @brief function that should run every loop
*/
void consoleLoop() {
    gameLoops[activeGame]();
}

void setupConsole(uint8_t clk, uint8_t di, uint8_t cs, uint16_t screenW, uint16_t screenH)
{
    setupIO();
    gl = new GL(clk, di, cs, screenW, screenH);

    gl->initGL();
    gl->clearDisplay();

    UITexture = (uint8_t*)malloc(screenW*screenH/8);

    screenWidth = screenW;
    screenHeight = screenH;

    gameSetups[CONSOLE_INTERFACE] = consoleInterfaceSetup;
    gameLoops[CONSOLE_INTERFACE] = consoleInterface;
    gameFinishes[CONSOLE_INTERFACE] = consoleInterfaceFinish;

    contextBuffer = gl->getDisplayBuffer();

    gameSetups[CONSOLE_INTERFACE]();
}

#endif