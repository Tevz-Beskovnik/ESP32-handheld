#include <keyboard.hpp>

Keyboard::Keyboard()
{
    ;
}

Keyboard::Keyboard(uint8_t x, uint8_t y, bool caps)
    : cursor_x(x), cursor_y(y), caps(caps)
{
    ;
}

char Keyboard::prompt()
{
    // register interupts at start of promt
    register_interupts();

    uint8_t io_event = 
    while()

    //unregister interupts at end of prompt
    remove_interupts();
}

char* Keyboard::prompt_string(uint8_t length)
{
    // register interupts at start of promt
    register_interupts();

    //unregister interupts at end of prompt
    remove_interupts();
}

uint64_t Keyboard::prompt_number()
{
    // register interupts at start of promt
    register_interupts();

    //unregister interupts at end of prompt
    remove_interupts();
}

void Keyboard::render_text_keyboard()
{

}
        
void Keyboard::render_numeric_keyboard()
{

}

void Keyboard::determin_action()
{

}