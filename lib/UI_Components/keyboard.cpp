#include <keyboard.hpp>

Keyboard::Keyboard(GL *gfx)
    : gl(gfx), input(255)
{
    ;
}

Keyboard::Keyboard(GL *gfx, bool caps)
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
    delay(200); // delay so that input of selecting mode doesnt get picked up
    // set the keyboard type
    keyboard_type = KB_TYPE_CHAR;

    // register interupts at start of promt
    register_interupts();

    Serial.println("Settingup interupts");

    line_break = LINE_BRAKE_CHAR;

    render_text_keyboard();

    // check for IO events (aka io interrupts)
    char res;
    while (true)
    {
        Serial.println("Waiting for evetns");
        input = check_io_interrupts();
        while (input == NO_IO_EVENT)
            input = check_io_interrupts();

        // determin action based on the input
        res = determin_action();

        if (res != NO_CHAR_SELECT)
            break;
    }

    Serial.println("Finished grabbing input");
    // unregister interupts at end of prompt
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
void Keyboard::prompt_string(char *out_string, uint8_t length)
{
    delay(200);

    for (uint8_t i = 0; i < length; i++)
        out_string[i] = ' ';

    keyboard_type = KB_TYPE_CHAR;

    // register interupts at start of promt
    register_interupts();

    char res;
    uint8_t counter = 0;

    line_break = LINE_BRAKE_CHAR;

    render_text_keyboard();

    while (true)
    {
        gl->setCursor(100, 27);
        gl->textColorBackground(BLACK, WHITE);
        for (uint8_t i = 0; i < length; i++)
            gl->print(out_string[i]);
        gl->refresh();

        input = check_io_interrupts();
        while (input == NO_IO_EVENT) // wait for a IO event to occure
            input = check_io_interrupts();

        res = determin_action();

        if (res != NO_CHAR_SELECT && res != INPUT_COMPLETE && res != BACKSPACE && counter != length) // check if res is a letter and save if it is
            out_string[counter++] = res;
        else if (counter == length)
            break;

        if (res == BACKSPACE && counter != 0)
            out_string[--counter] = ' ';

        if (res == INPUT_COMPLETE) // break the loop if input is compleate
            break;
    }

    // unregister interupts at end of prompt
    remove_interupts();
}

uint64_t Keyboard::prompt_number(uint8_t length)
{
    delay(200);

    uint8_t current_len = 0;

    uint64_t output = 0;

    char res;

    keyboard_type = KB_TYPE_NUM;
    // register interupts at start of promt
    register_interupts();

    render_numeric_keyboard();

    while (true)
    {
        input = check_io_interrupts();

        while (input != NO_IO_EVENT)
            input = check_io_interrupts();

        res = determin_action();

        if (res == NO_CHAR_SELECT || res != INPUT_COMPLETE && current_len != length)
        {
            output *= 10;
            output += (res - 30);
        }

        if (res == INPUT_COMPLETE)
            break;
    }

    // unregister interupts at end of prompt
    remove_interupts();

    return output;
}

void Keyboard::render_text_keyboard()
{
    Serial.println("Rendering keyboard");

    uint8_t line_number = 0;

    gl->fillRect(90, 70, 210, 170, BLACK);
    gl->fillRect(90, 20, 210, 40, BLACK);
    gl->fontSize(FONT_SIZE_3);
    gl->textColor(BLACK);

    for (uint8_t i = 0; i < char_k_len; i++)
    {
        if (i == cursor_pos)
            gl->textColorBackground(WHITE, BLACK);
        else
            gl->textColorBackground(BLACK, WHITE);

        gl->setCursor(100 + ((i - line_number * line_break) * 19), 80 + line_number * 25);
        gl->print((char)(char_keyboard[i] - ((caps * 32) * (i > 9) * (i < 36))));

        if ((i + 1) % line_break == 0)
            line_number++;
    }
    gl->refresh();
}

void Keyboard::render_numeric_keyboard()
{
    uint8_t line_number = 0;

    gl->drawRect(0, 0, 400, 240, WHITE);
    gl->fontSize(FONT_SIZE_3);
    gl->textColor(BLACK);

    for (uint8_t i = 0; i < num_k_len; i++)
    {
        if (i == cursor_pos)
            gl->textColorBackground(WHITE, BLACK);
        else
            gl->textColorBackground(BLACK, WHITE);

        gl->setCursor(100 + i * 19, 90 + line_number * 25);
        gl->print(num_keyboard[i]);

        if (i % line_break == 0)
        {

            line_number++;
        }
    }
}

/**
 * @brief based on the last input from the console io determens what action to take
 *
 * @returns character that was selected on keyboard else 255
 */
char Keyboard::determin_action()
{
    line_break = LINE_BRAKE_CHAR;

    if (keyboard_type == KB_TYPE_NUM)
        line_break = LINE_BRAKE_NUM;

    switch (input)
    {
    case BUTTON_UP:
        increment_cursor_and_render(-(int32_t)line_break);
        break;
    case BUTTON_DOWN:
        increment_cursor_and_render((int32_t)line_break);
        break;
    case BUTTON_LEFT:
        increment_cursor_and_render(-1);
        break;
    case BUTTON_RIGHT:
        increment_cursor_and_render(1);
        break;
    case BUTTON_A:
        caps = !caps;
        render_text_keyboard();
        break;
    case BUTTON_B:
        return get_selected_character();
        break;
    }
    return NO_CHAR_SELECT;
}

inline void Keyboard::increment_cursor_and_render(int32_t add)
{
    uint8_t k_len = (keyboard_type == KB_TYPE_CHAR ? char_k_len : num_k_len);

    cursor_pos += k_len + add;
    cursor_pos %= k_len;

    if (keyboard_type == KB_TYPE_CHAR)
        render_text_keyboard();
    else
        render_numeric_keyboard();
}

char Keyboard::get_selected_character()
{
    if (keyboard_type == KB_TYPE_CHAR)
        return (char_keyboard[cursor_pos] - (0x20 * caps) * (cursor_pos > 9) * (cursor_pos < 36)) * (cursor_pos != KB_END && cursor_pos != KB_BACKSPACE) + (INPUT_COMPLETE) * (cursor_pos == KB_END) + (BACKSPACE) * (cursor_pos == KB_BACKSPACE);
    else
        return num_keyboard[cursor_pos];
}