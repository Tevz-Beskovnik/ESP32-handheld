#include <Console-UI.hpp>

void gameButton(Active *object, uint8_t color, const char *text, uint8_t i)
{
    Event ev;
    ev.event_type = FILL_RECT;
    ev.params[0] = 149;
    ev.params[1] = 59 + (i * 25);
    ev.params[2] = (strlen(text) * 12) + 10;
    ev.params[3] = 20;
    ev.params[4] = !color;
    active_object_post(object, ev);
    // gfx->fillRect(149, 59 + (i * 25), (strlen(text) * 12) + 10, 20, !color);
    ev.params[0] = 151;
    ev.params[1] = 65 + (i * 25);
    ev.params[2] = 6;
    ev.params[3] = 6;
    ev.params[4] = color;
    active_object_post(object, ev);
    // gfx->fillRect(151, 65 + (i * 25), 6, 6, color);
    ev.event_type = SET_CURSOR;
    ev.params[0] = 160;
    ev.params[1] = 61 + (i * 25);
    active_object_post(object, ev);
    // gfx->setCursor(160, 61 + (i * 25));
    ev.event_type = FONT_SIZE;
    ev.params[0] = FONT_SIZE_2;
    active_object_post(object, ev);
    // gfx->fontSize(FONT_SIZE_2);
    ev.event_type = TEXT_COLOR;
    ev.params[0] = color;
    active_object_post(object, ev);
    // gfx->textColor(color);
    ev.event_type = PRINT;
    ev.misc = (void *)text;
    active_object_post(object, ev);
    // gfx->printf(text);
}

ConsoleUI::ConsoleUI(uint8_t *callback)
    : Application("UI"), callback(callback), number_of_games(0), selected_game(0), refresh_counter(0)
{
    ;
}

void ConsoleUI::add_name(const char *name)
{
    game_names[number_of_games] = name;
    number_of_games++;
}

void ConsoleUI::setup()
{
    uint8_t **callback = new uint8_t *;
    Event ev;
    ev.event_type = LOAD_TEXTURE;
    ev.misc = (void *)controlsSprite;
    ev.params[0] = 96;
    ev.params[1] = 96;
    ev.params[2] = TEXTURE_BINDING_0;
    ev.params[3] = 0;
    active_object_post(object, ev);
    // gfx->loadTexture(controlsSprite, 96, 96, TEXTURE_BINDING_0);

    ev.event_type = CLEAR_DISPLAY_BUFFER;
    active_object_post(object, ev);
    // gfx->clearDisplayBuffer();

    ev.event_type = ALLOCATE_TEXTURE;
    ev.params[0] = 400;
    ev.params[1] = 240;
    ev.params[2] = TEXTURE_BINDING_1;
    active_object_post(object, ev);
    // gfx->allocateTexture(400, 240, TEXTURE_BINDING_1);
    ev.event_type = CHANGE_CONTEXT;
    ev.misc = (void *)callback;
    ev.params[0] = TEXTURE_BINDING_1;
    active_object_post(object, ev);
    // gfx->changeContext(TEXTURE_BINDING_1);

    ev.event_type = FILL_RECT;
    ev.params[0] = 0;
    ev.params[1] = 0;
    ev.params[2] = 400;
    ev.params[3] = 240;
    ev.params[4] = WHITE;
    active_object_post(object, ev);
    // gfx->fillRect(0, 0, 400, 240, WHITE);
    ev.event_type = FILL_RECT_D;
    ev.params[0] = 4;
    ev.params[1] = 4;
    ev.params[2] = 390;
    ev.params[3] = 230;
    active_object_post(object, ev);
    // gfx->fillRectD(4, 4, 390, 230);
    ev.event_type = FILL_RECT;
    ev.params[0] = 9;
    ev.params[1] = 9;
    ev.params[2] = 380;
    ev.params[3] = 220;
    ev.params[4] = BLACK;
    active_object_post(object, ev);
    // gfx->fillRect(9, 9, 380, 220, BLACK);
    ev.event_type = FILL_RECT_D;
    ev.params[0] = 4;
    ev.params[1] = 4;
    ev.params[2] = 110;
    ev.params[3] = 20;
    active_object_post(object, ev);
    // gfx->fillRectD(4, 4, 110, 20);
    ev.event_type = FILL_RECT;
    ev.params[0] = 0;
    ev.params[1] = 0;
    ev.params[2] = 110;
    ev.params[3] = 20;
    ev.params[4] = WHITE;
    active_object_post(object, ev);
    // gfx->fillRect(0, 0, 110, 20, WHITE);
    ev.event_type = FILL_TRIANGLE_D;
    ev.params[0] = 99;
    ev.params[1] = 23;
    ev.params[2] = 114;
    ev.params[3] = 24;
    ev.params[4] = 114;
    ev.params[5] = 9;
    active_object_post(object, ev);
    // gfx->fillTriangleD(99, 23, 114, 24, 114, 9);
    ev.event_type = FILL_TRIANGLE;
    ev.params[0] = 104;
    ev.params[1] = 24;
    ev.params[2] = 114;
    ev.params[3] = 25;
    ev.params[4] = 115;
    ev.params[5] = 14;
    ev.params[6] = BLACK;
    active_object_post(object, ev);
    // gfx->fillTriangle(104, 24, 114, 25, 115, 14, BLACK);
    ev.event_type = SET_CURSOR;
    ev.params[0] = 20;
    ev.params[1] = 8;
    active_object_post(object, ev);
    // gfx->setCursor(20, 8);
    ev.event_type = FONT_SIZE;
    ev.params[0] = 1;
    active_object_post(object, ev);
    // gfx->fontSize(1);
    ev.event_type = TEXT_COLOR;
    ev.params[0] = BLACK;
    active_object_post(object, ev);
    // gfx->textColor(BLACK);
    ev.event_type = PRINT;
    ev.misc = (void *)"ESP-SHITBOARD";
    active_object_post(object, ev);
    // gfx->print("ESP-SHITBOARD");

    ev.event_type = FILL_RECT;
    ev.params[0] = 19;
    ev.params[1] = 34;
    ev.params[2] = 120;
    ev.params[3] = 186;
    ev.params[4] = WHITE;
    active_object_post(object, ev);
    // gfx->fillRect(19, 34, 120, 186, WHITE);
    ev.event_type = SET_CURSOR;
    ev.params[0] = 28;
    ev.params[1] = 40;
    active_object_post(object, ev);
    // gfx->setCursor(28, 40);
    ev.event_type = FONT_SIZE;
    ev.params[0] = 2;
    active_object_post(object, ev);
    // gfx->fontSize(2);
    ev.event_type = PRINT;
    ev.misc = (void *)"Controls:";
    active_object_post(object, ev);
    // gfx->print("Controls:");
    ev.event_type = DRAW_TEXTURE;
    ev.params[0] = 25;
    ev.params[1] = 60;
    ev.params[2] = TEXTURE_BINDING_0;
    active_object_post(object, ev);
    // gfx->drawTexture(25, 60, TEXTURE_BINDING_0);
    ev.event_type = SET_CURSOR;
    ev.params[0] = 22;
    ev.params[1] = 140;
    active_object_post(object, ev);
    // gfx->setCursor(22, 140);
    ev.event_type = PRINT_LN;
    ev.misc = (void *)"Arrow key:";
    active_object_post(object, ev);
    // gfx->println("Arrow key:");
    ev.event_type = SET_CURSOR;
    ev.params[0] = 25;
    ev.params[1] = 156;
    active_object_post(object, ev);
    // gfx->setCursor(25, 156);
    ev.event_type = PRINT_LN;
    ev.misc = (void *)"Selection";
    active_object_post(object, ev);
    // gfx->println("Selection");
    ev.event_type = SET_CURSOR;
    ev.params[0] = 22;
    ev.params[1] = 178;
    active_object_post(object, ev);
    // gfx->setCursor(22, 178);
    ev.event_type = PRINT_LN;
    ev.misc = (void *)"A button:";
    active_object_post(object, ev);
    // gfx->println("A button:");
    ev.event_type = SET_CURSOR;
    ev.params[0] = 25;
    ev.params[1] = 194;
    active_object_post(object, ev);
    // gfx->setCursor(25, 194);
    ev.event_type = PRINT;
    ev.misc = (void *)"Confirm";
    active_object_post(object, ev);
    // gfx->print("Confirm");

    ev.event_type = SET_CURSOR;
    ev.params[0] = 150;
    ev.params[1] = 35;
    active_object_post(object, ev);
    // gfx->setCursor(150, 35);
    ev.event_type = TEXT_COLOR;
    ev.params[0] = WHITE;
    active_object_post(object, ev);
    // gfx->textColor(WHITE);
    ev.event_type = PRINT;
    ev.misc = (void *)"Applications:";
    active_object_post(object, ev);
    // gfx->print("Applications:");

    ev.event_type = CHANGE_CONTEXT;
    ev.params[0] = CONTEXT_BUFFER;
    ev.misc = (void *)callback;
    active_object_post(object, ev);
    // gfx->changeContext(CONTEXT_BUFFER);*/

    delete callback;
}

bool ConsoleUI::loop()
{
    Event ev;

    refresh_counter++;

    if (selected_game > 0 && is_pressed_sticky(BUTTON_UP_ID))
    {
        selected_game--;
    }

    if (selected_game < number_of_games - 1 && is_pressed_sticky(BUTTON_DOWN_ID))
    {
        selected_game++;
    }

    if (is_pressed_sticky(BUTTON_A_ID))
    {
        *callback = selected_game + 1;
        return false;
    }

    if (refresh_counter == 16000)
    {
        refresh_counter = 0;

        ev.event_type = CLEAR_DISPLAY_BUFFER;
        active_object_post(object, ev);
        // gfx->clearDisplayBuffer();

        ev.event_type = DRAW_TEXTURE;
        ev.params[0] = 0;
        ev.params[1] = 0;
        ev.params[2] = TEXTURE_BINDING_1;
        active_object_post(object, ev);
        // gfx->drawTexture(0, 0, TEXTURE_BINDING_1);

        for (uint8_t i = 0; i < number_of_games; i++)
        {
            gameButton(object, selected_game == i ? BLACK : WHITE, (const char *)game_names[i], i);
        }

        ev.event_type = REFRESH;
        active_object_post(object, ev);
        // gfx->refresh();
    }

    return true;
}

void ConsoleUI::cleanup()
{
    Event ev;
    ev.event_type = CLEAR_TEXTURE;
    ev.params[0] = TEXTURE_BINDING_0;
    active_object_post(object, ev);
    // gfx->clearTexture(TEXTURE_BINDING_0);
    ev.params[0] = TEXTURE_BINDING_1;
    active_object_post(object, ev);
    // gfx->clearTexture(TEXTURE_BINDING_1);
}