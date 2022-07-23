#include <Console-Core.h>


void initConsole()
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