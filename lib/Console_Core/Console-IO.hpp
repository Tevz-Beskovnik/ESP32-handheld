/**
 * @author: Tevž Beškovnik
 * @date: 23. 7. 2022
 * @description: Iot stuff, not including screen....
 */

/**
 * TODO:
 * Re-write this whole shite as a class
 * too much shit with the lincer having all kinds of redefenitions
 * just stick to the C++ coding convetions
 * my fucking head hurst couse of this shit
 */

#pragma once

#include <FreeRTOSConfig.h>
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "soc/gpio_struct.h"
#include "driver/gpio.h"
#include "freertos/queue.h"

#define BUTTON_UP_ID 1
#ifndef BUTTON_UP
#define BUTTON_UP 35
#endif

#define BUTTON_DOWN_ID 0
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

struct ButtonInfo
{
    uint8_t pinNum;
    uint8_t state;
};

static void IRAM_ATTR io_interrupt_handler(void *args);

/**
 * @brief: setsup the io of the console
 */
void setupIO();

/**
 * @brief checks if the buttons is pressed
 *
 * @param id of button to check, by default set to BUTTON_UP_ID
 */
bool is_pressed(uint8_t id = BUTTON_UP_ID);

/**
 * @brief check if button was clicked returns 1 once then 0 if the button is still pressed
 *
 * @param id of button to check, by default set to BUTTON_UP_ID
 */
bool is_pressed_sticky(uint8_t id = BUTTON_UP_ID);

/**
 * @brief registers interrupts for console IO intputs
 */
void register_interupts();

/**
 * @brief check if any GPIO interupts happened if true will return number of pin return 255 if no buttons were pressed
 */
uint8_t check_io_interrupts();

/**
 * @brief unregisters all console IO interrupts
 */
void remove_interupts();
