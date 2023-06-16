#include <button.hpp>

void button(GL *gfx, uint16_t x, uint16_t y, uint8_t color, const char *text)
{
    gfx->fillRect(x, y, (strlen(text) * 12) + 10, 20, !color);
    gfx->fillRect(x + 2, y + 6, 6, 6, color);
    gfx->setCursor(x + 11, y + 2);
    gfx->fontSize(FONT_SIZE_2);
    gfx->textColor(color);
    gfx->printf(text);
}