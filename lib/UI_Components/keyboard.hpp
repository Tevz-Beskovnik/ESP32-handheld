
#include <GL.hpp>
#include <Console-Core.hpp>

// what type of keyboard to render
#ifndef _KEYBOARD_
#define _KEYBOARD_

#define KB_TYPE_NUM 1
#define KB_TYPE_CHAR 0

// actions
#define NO_CHAR_SELECT 0xff
#define INPUT_COMPLETE 0x01
class Keyboard
{
    public:
        Keyboard(GL* gfx);

        Keyboard(GL* gfx, bool caps);

        char prompt();

        void prompt_string(char* out_string, uint8_t length);

        uint64_t prompt_number(uint8_t length);
    private:
        void render_text_keyboard(char* string, uint8_t length); // draws the keyboard variant with letters and the current string to display
        
        void render_numeric_keyboard(char* string, uint8_t length); // draws the numbered keyboard variant

        char determin_action(); // determin what action to take from gotten input

        void increment_cursor_and_render(uint8_t current, int32_t add);

        char get_selected_character();

        uint8_t cursor_pos = 0;

        uint8_t line_break; // at how many characters to brake the line at

        uint8_t input; // current input gotten from console io

        uint8_t keyboard_type;

        GL* gl = nullptr;

        uint8_t char_k_len = 27;
        uint8_t num_k_len = 10;
        char char_keyboard[27] = { 'q', 'w', 'e', 'r', 't', 'z', 'u', 'i', 'o', 'p', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'y', 'x', 'c', 'v', 'b', 'n', 'm', ' '};
        char num_keyboard[10] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};

        bool caps = false;
};

#endif