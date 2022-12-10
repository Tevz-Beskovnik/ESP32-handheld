/**
 * @author: Tevž Beškovnik
 * @date: 23. 7. 2022
 * @description: Iot stuff, not including screen....
*/

#pragma once

#include <FreeRTOSConfig.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "soc/gpio_struct.h"
#include "driver/gpio.h"

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

#define NO_IO_EVENT 0xff

struct ButtonInfo {
    uint8_t pinNum;
    uint8_t state;
} Buttons[6];
xQueueHandle interuptQueue;

static void IRAM_ATTR io_interrupt_handler(void *args)
{
    int pinNum = (int)args;
    xQueueSendFromISR(interuptQueue, &pinNum, NULL);
}

/**
 * @brief: setsup the io of the console
*/
void setupIO()
{
    gpio_pad_select_gpio(BUTTON_UP);
    gpio_set_direction((gpio_num_t)BUTTON_UP, GPIO_MODE_INPUT);
    gpio_pad_select_gpio(BUTTON_DOWN);
    gpio_set_direction((gpio_num_t)BUTTON_DOWN, GPIO_MODE_INPUT);
    gpio_pad_select_gpio(BUTTON_LEFT);
    gpio_set_direction((gpio_num_t)BUTTON_LEFT, GPIO_MODE_INPUT);
    gpio_pad_select_gpio(BUTTON_RIGHT);
    gpio_set_direction((gpio_num_t)BUTTON_RIGHT, GPIO_MODE_INPUT);
    gpio_pad_select_gpio(BUTTON_A);
    gpio_set_direction((gpio_num_t)BUTTON_A, GPIO_MODE_INPUT);
    gpio_pad_select_gpio(BUTTON_B);
    gpio_set_direction((gpio_num_t)BUTTON_B, GPIO_MODE_INPUT);

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

/**
 * @brief checks if the buttons is pressed
 * 
 * @param id of button to check, by default set to BUTTON_UP_ID
*/
bool is_pressed(uint8_t id = BUTTON_UP_ID)
{
    return gpio_get_level((gpio_num_t)Buttons[id].pinNum);
}

/**
 * @brief check if button was clicked returns 1 once then 0 if the button is still pressed
 * 
 * @param id of button to check, by default set to BUTTON_UP_ID
*/
bool is_pressed_sticky(uint8_t id = BUTTON_UP_ID)
{
    int currentState = gpio_get_level((gpio_num_t)Buttons[id].pinNum);
    if(Buttons[id].state == 0 && currentState == 1)
    {
        Buttons[id].state = currentState;
        return true;
    }
    Buttons[id].state = currentState;
    return false;
}

/**
 * @brief registers interrupts for console IO intputs
*/
void register_interupts()
{
    esp_err_t err;

    err = gpio_install_isr_service(0);
    assert(err == ESP_OK);

    interuptQueue = xQueueCreate(10, sizeof(int));

    for(uint8_t i = 0; i < 6; i++)
    {
        gpio_pad_select_gpio((gpio_num_t)Buttons[i].pinNum);
        err = gpio_set_intr_type((gpio_num_t)Buttons[i].pinNum, GPIO_INTR_POSEDGE);
        assert(err == ESP_OK);

        err = gpio_isr_handler_add((gpio_num_t)Buttons[i].pinNum, io_interrupt_handler, (void *)&Buttons[i].pinNum);
        assert(err == ESP_OK);
    }
}

/**
 * @brief check if any GPIO interupts happened if true will return number of pin return 255 if no buttons were pressed
*/
uint8_t check_io_interrupts()
{
    for(uint8_t i = 0; i < 6; i++)
    {
        if(xQueueReceive(interuptQueue, &Buttons[i].pinNum, portMAX_DELAY))
            return Buttons[i].pinNum;
    }
    return NO_IO_EVENT;
}

/**
 * @brief unregisters all console IO interrupts
*/
void remove_interupts()
{
    esp_err_t err;
    for(uint8_t i = 0; i < 6; i++)
    {
        err = gpio_isr_handler_remove((gpio_num_t)Buttons[i].pinNum);
        assert(err == ESP_OK);
    }

    vQueueDelete(interuptQueue);
    gpio_uninstall_isr_service();
}
