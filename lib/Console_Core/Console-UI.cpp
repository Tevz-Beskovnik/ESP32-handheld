#include <Console-UI.hpp>

void gameButton(GL *gfx, uint8_t color, const char *text, uint8_t i)
{
    gfx->fillRect(149, 59 + (i * 25), (strlen(text) * 12) + 10, 20, !color);
    gfx->fillRect(151, 65 + (i * 25), 6, 6, color);
    gfx->setCursor(160, 61 + (i * 25));
    gfx->fontSize(FONT_SIZE_2);
    gfx->textColor(color);
    gfx->printf(text);
}

ConsoleUI::ConsoleUI(uint8_t *callback)
    : Application("UI"), callback(callback), number_of_games(0), selected_game(0)
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
    gfx->loadTexture(controlsSprite, 96, 96, TEXTURE_BINDING_0);

    gfx->clearDisplayBuffer();

    gfx->allocateTexture(400, 240, TEXTURE_BINDING_1);
    gfx->changeContext(TEXTURE_BINDING_1);

    gfx->fillRect(0, 0, 400, 240, WHITE);
    gfx->fillRectD(4, 4, 390, 230);
    gfx->fillRect(9, 9, 380, 220, BLACK);
    gfx->fillRectD(4, 4, 110, 20);
    gfx->fillRect(0, 0, 110, 20, WHITE);
    gfx->fillTriangleD(99, 23, 114, 24, 114, 9);
    gfx->fillTriangle(104, 24, 114, 25, 115, 14, BLACK);
    gfx->setCursor(20, 8);
    gfx->fontSize(1);
    gfx->textColor(BLACK);
    gfx->print("ESP-SHITBOARD");

    gfx->fillRect(19, 34, 120, 186, WHITE);
    gfx->setCursor(28, 40);
    gfx->fontSize(2);
    gfx->print("Controls:");
    gfx->drawTexture(25, 60, TEXTURE_BINDING_0);
    gfx->setCursor(22, 140);
    gfx->println("Arrow key:");
    gfx->setCursor(25, 156);
    gfx->println("Selection");
    gfx->setCursor(22, 178);
    gfx->println("A button:");
    gfx->setCursor(25, 194);
    gfx->print("Confirm");

    gfx->setCursor(150, 35);
    gfx->textColor(WHITE);
    gfx->print("Applications:");

    gfx->changeContext(CONTEXT_BUFFER);
}

bool ConsoleUI::loop()
{
    gfx->clearDisplayBuffer();

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

    gfx->drawTexture(0, 0, TEXTURE_BINDING_1);

    for (uint8_t i = 0; i < number_of_games; i++)
    {
        gameButton(gfx, selected_game == i ? BLACK : WHITE, (const char *)game_names[i], i);
    }

    gfx->refresh();

    return true;
}

void ConsoleUI::cleanup()
{
    gfx->clearTexture(TEXTURE_BINDING_0);
    gfx->clearTexture(TEXTURE_BINDING_1);
}