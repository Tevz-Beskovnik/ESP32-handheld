
#include <GL.hpp>
#include <Console-IO.hpp>

// what type of keyboard to render
#ifndef _KEYBOARD_
#define _KEYBOARD_

#define KB_TYPE_NUM 1
#define KB_TYPE_CHAR 0

// actions
#define NO_CHAR_SELECT 0xff
#define INPUT_COMPLETE 0x01
#define BACKSPACE 0x02

#define LINE_BRAKE_CHAR 10
#define LINE_BRAKE_NUM 3

#define KB_END 55
#define KB_BACKSPACE 56

class Keyboard
{
public:
    explicit Keyboard(GL *gfx);

    Keyboard(GL *gfx, bool caps);

    char prompt();

    void prompt_string(char *out_string, uint8_t length);

    uint64_t prompt_number(uint8_t length);

private:
    void render_text_keyboard(); // draws the keyboard variant with letters and the current string to display

    void render_numeric_keyboard(); // draws the numbered keyboard variant

    char determin_action(); // determin what action to take from gotten input

    void increment_cursor_and_render(int32_t add);

    char get_selected_character();

    uint16_t cursor_pos = 0;

    uint8_t line_break; // at how many characters to brake the line at

    uint8_t input; // current input gotten from console io

    uint8_t keyboard_type;

    GL *gl = nullptr;

    uint8_t char_k_len = 57;
    uint8_t num_k_len = 12;
    char char_keyboard[57] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'q', 'w', 'e', 'r', 't', 'z', 'u', 'i', 'o', 'p', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'y', 'x', 'c', 'v', 'b', 'n', 'm', ' ', ',', '.', '-', '+', '#', ';', ':', '_', '<', '>', '!', '?', '=', '(', ')', '&', '/', '*', 251, 0xAE};
    char num_keyboard[12] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 251, 0xAE};

    bool caps = false;
};

#endif