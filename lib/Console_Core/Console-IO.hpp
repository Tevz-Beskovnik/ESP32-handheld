#ifndef _CONSOLE_IO_
#define _CONSOLE_IO_

#include <FreeRTOSConfig.h>
#include <Arduino.h>

#define BUTTON_UP_ID 0
#ifndef BUTTON_UP
#define BUTTON_UP 35
#endif

#define BUTTON_DOWN_ID 1
#ifndef BUTTON_DOWN
#define BUTTON_DOWN 33
#endif

#define BUTTON_LEFT_ID 2
#ifndef BUTTON_LEFT
#define BUTTON_LEFT 34
#endif

#define BUTTON_RIGHT_ID 3
#ifndef BUTTON_RIGHT
#define BUTTON_RIGHT 32
#endif

#define BUTTON_A_ID 4
#ifndef BUTTON_A
#define BUTTON_A 12
#endif

#define BUTTON_B_ID 5
#ifndef BUTTON_B
#define BUTTON_B 14
#endif

struct ButtonInfo {
    uint8_t pinNum;
    uint8_t state;
} Buttons[6];

void setupIO()
{
    pinMode(BUTTON_UP, INPUT);
    pinMode(BUTTON_DOWN, INPUT);
    pinMode(BUTTON_LEFT, INPUT);
    pinMode(BUTTON_RIGHT, INPUT);
    pinMode(BUTTON_A, INPUT);
    pinMode(BUTTON_B, INPUT);

    Buttons[BUTTON_UP_ID].pinNum = BUTTON_UP;
    Buttons[BUTTON_UP_ID].state = 0;
    Buttons[BUTTON_DOWN_ID].pinNum = BUTTON_DOWN;
    Buttons[BUTTON_DOWN_ID].state = 0;
    Buttons[BUTTON_LEFT_ID].pinNum = BUTTON_LEFT;
    Buttons[BUTTON_LEFT_ID].state = 0;
    Buttons[BUTTON_RIGHT_ID].pinNum = BUTTON_RIGHT;
    Buttons[BUTTON_RIGHT_ID].state = 0;
    Buttons[BUTTON_A_ID].pinNum = BUTTON_A;
    Buttons[BUTTON_A_ID].state = 0;
    Buttons[BUTTON_B_ID].pinNum = BUTTON_B;
    Buttons[BUTTON_B_ID].state = 0;
}

bool isPressed(uint8_t id = BUTTON_UP_ID)
{
    return digitalRead(Buttons[id].pinNum);
}

bool isPressedSticky(uint8_t id = BUTTON_UP_ID)
{
    int currentState = digitalRead(Buttons[id].pinNum);
    if(Buttons[id].state == 0 && currentState == 1)
    {
        Buttons[id].state = currentState;
        return true;
    }
    Buttons[id].state = currentState;
    return false;
}

#endif