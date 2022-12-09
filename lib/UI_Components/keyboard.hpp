#include <GL.hpp>
#include <Console-IO.hpp>

class Keyboard
{
    public:
        Keyboard();

        Keyboard(uint8_t x, uint8_t y, bool caps);

        char prompt();

        char* prompt_string(uint8_t length);

        uint64_t prompt_number();
    private:
        void render_text_keyboard(); // draws the keyboard variant with letters
        
        void render_numeric_keyboard(); // draws the numbered keyboard variant

        void determin_action(); // determin what action to take from gotten input

        uint8_t cursor_x = 0;
        uint8_t cursor_y = 0;

        char* char_keyboard[27];
        char num_keyboard[10] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};

        bool caps = false;
};