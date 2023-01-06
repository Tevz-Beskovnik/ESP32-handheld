#include <keyboard.hpp>

Keyboard::Keyboard(GL* gfx)
    : gl(gfx), input(255)
{
    ;
}

Keyboard::Keyboard(GL* gfx, bool caps)
    : gl(gfx), cursor_pos(0), caps(caps), input(255)
{
    ;
}

/**
 * @brief opens keyboard and prompts for 1 character
 * 
 * @returns a single character
*/
char Keyboard::prompt()
{
    //set the keyboard type
    keyboard_type = KB_TYPE_CHAR;

    // register interupts at start of promt
    register_interupts();
    
    // check for IO events (aka io interrupts)
    char res;
    while(true)
    {
        input = check_io_interrupts();
        while(input == NO_IO_EVENT)
            input = check_io_interrupts();

        // determin action based on the input
        res = determin_action();

        if(res != NO_CHAR_SELECT)
            break;
    }

    //unregister interupts at end of prompt
    remove_interupts();

    return res;
}

/**
 * @brief prompt for a string opens a keyboard
 * 
 * @param out_string the buffer the string should be stored to
 * 
 * @param length the size of the buffer and maximum lenght of string that can be returned
*/
void Keyboard::prompt_string(char* out_string, uint8_t length)
{
    // register interupts at start of promt
    register_interupts();

    char res;
    uint8_t counter = 0;
    while(true)
    {
        input = check_io_interrupts();
        while(input == NO_IO_EVENT) // wait for a IO event to occure
            input = check_io_interrupts();

        res = determin_action();
        if(res != NO_CHAR_SELECT && res != INPUT_COMPLETE && counter != length) // check if res is a letter and save if it is
            out_string[++counter] = res;
        else if(counter == length)
            break;

        if(res == INPUT_COMPLETE) // break the loop if input is compleate
            break;
    }

    //unregister interupts at end of prompt
    remove_interupts();
}

uint64_t Keyboard::prompt_number(uint8_t length)
{
    // register interupts at start of promt
    register_interupts();

    //unregister interupts at end of prompt
    remove_interupts();
}

void Keyboard::render_text_keyboard(char* string = nullptr, uint8_t length = 0)
{
    uint8_t line_number = 0;

    gl->drawRect(0,0, 400, 240, WHITE);
    gl->fontSize(FONT_SIZE_3);
    gl->textColor(BLACK);

    if(string != nullptr)
        gl->print(string);
    
    for(uint8_t i = 0; i < char_k_len; i++)
    {
        if(i == cursor_pos)
            gl->textColor(WHITE);
        else
            gl->textColor(BLACK);

        gl->setCursor(i*19, 100+line_number*25);
        gl->print(char_keyboard[i]);

        if(i % line_break == 0)
            line_number++;
    }
}
        
void Keyboard::render_numeric_keyboard(char* string = nullptr, uint8_t length = 0)
{
    uint8_t line_number = 0;

    gl->drawRect(0,0, 400, 240, WHITE);
    gl->fontSize(FONT_SIZE_3);
    gl->textColor(BLACK);

    for(uint8_t i = 0; i < num_k_len; i++)
    {
        if(i == cursor_pos)
            gl->textColor(WHITE);
        else
            gl->textColor(BLACK);
        
        gl->setCursor(100+i*19, 90+line_number*25);
        gl->print(num_keyboard[i]);

        if(i % line_break == 0)
            line_number++;
    }
}

/**
 * @brief based on the last input from the console io determens what action to take
 * 
 * @returns character that was selected on keyboard else 255
*/
char Keyboard::determin_action()
{
    line_break = 7;
    
    if(keyboard_type == KB_TYPE_NUM)
        line_break = 3;

    switch (input)
    {
        case BUTTON_UP:
            increment_cursor_and_render(cursor_pos, -((int32_t)line_break));
            break;
        case BUTTON_DOWN:
            increment_cursor_and_render(cursor_pos, ((int32_t)line_break));
            break;
        case BUTTON_LEFT:
            increment_cursor_and_render(cursor_pos, -1);
            break;
        case BUTTON_RIGHT:
            increment_cursor_and_render(cursor_pos, 1);
            break;
        case BUTTON_A:
            caps = true;
            break;
        case BUTTON_B:
            return get_selected_character();
            break;
    }
    return NO_CHAR_SELECT;
}

inline void Keyboard::increment_cursor_and_render(uint8_t current, int32_t add)
{
    uint8_t k_len = (keyboard_type == KB_TYPE_CHAR ? char_k_len : num_k_len);

    if((int32_t)((int32_t)current + add) < 0)
        cursor_pos = k_len + (int32_t)((int32_t)current + add);
    else if(current + add >= k_len)
        cursor_pos = (current + add) - k_len;
    else
        cursor_pos = current + add;

    if(keyboard_type == KB_TYPE_CHAR)
        render_text_keyboard();
    else
        render_numeric_keyboard();
}

char Keyboard::get_selected_character()
{
    if(keyboard_type == KB_TYPE_CHAR)
        return caps ? char_keyboard[cursor_pos] - 0x20 : char_keyboard[cursor_pos];
    else
        return num_keyboard[cursor_pos];
}