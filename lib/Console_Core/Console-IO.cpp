#include <Console-IO.hpp>

static xQueueHandle interuptQueue;

static ButtonInfo Buttons[6];

void io_interrupt_handler(void *args)
{
    uint8_t pinNum = *((uint8_t *)args);
    Serial.printf("Got key: %i\n", (int)pinNum);
    xQueueSendFromISR(interuptQueue, &pinNum, NULL);
}

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

bool is_pressed(uint8_t id)
{
    return gpio_get_level((gpio_num_t)Buttons[id].pinNum);
}

bool is_pressed_sticky(uint8_t id)
{
    int currentState = gpio_get_level((gpio_num_t)Buttons[id].pinNum);
    if (Buttons[id].state == 0 && currentState == 1)
    {
        Buttons[id].state = currentState;
        return true;
    }
    Buttons[id].state = currentState;
    return false;
}

void register_interupts()
{
    esp_err_t err;

    err = gpio_install_isr_service(0);
    assert(err == ESP_OK);

    interuptQueue = xQueueCreate(10, sizeof(int));

    for (uint8_t i = 0; i < 6; i++)
    {
        gpio_pad_select_gpio((gpio_num_t)Buttons[i].pinNum);
        err = gpio_set_intr_type((gpio_num_t)Buttons[i].pinNum, GPIO_INTR_POSEDGE);
        assert(err == ESP_OK);

        err = gpio_isr_handler_add((gpio_num_t)Buttons[i].pinNum, io_interrupt_handler, (void *)&Buttons[i].pinNum);
        assert(err == ESP_OK);
    }
}

uint8_t check_io_interrupts()
{
    uint8_t pinNum;
    for (uint8_t i = 0; i < 6; i++)
    {
        if (xQueueReceive(interuptQueue, &pinNum, portMAX_DELAY))
            return pinNum;
    }
    return NO_IO_EVENT;
}

void remove_interupts()
{
    esp_err_t err;
    for (uint8_t i = 0; i < 6; i++)
    {
        gpio_pad_select_gpio((gpio_num_t)Buttons[i].pinNum);
        err = gpio_set_intr_type((gpio_num_t)Buttons[i].pinNum, GPIO_INTR_DISABLE);
        assert(err == ESP_OK);

        err = gpio_isr_handler_remove((gpio_num_t)Buttons[i].pinNum);
        assert(err == ESP_OK);
    }

    vQueueDelete(interuptQueue);
    gpio_uninstall_isr_service();

    setupIO();
}