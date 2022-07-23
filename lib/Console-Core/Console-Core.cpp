#include <Console-Core.h>


void setupConsole()
{
    
}

void registerGameLoop(GameLoop_t gameLoopN)
{
    gameLoop = gameLoopN;
}

void drawLoop()
{
    DrawInstruction instruction = pop(&pushHeap);
}

void consoleLogic();