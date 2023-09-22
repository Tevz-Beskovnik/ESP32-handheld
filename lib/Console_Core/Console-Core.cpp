#include <Console-Core.hpp>

Console::Console(uint8_t clk, uint8_t di, uint8_t cs, uint16_t screen_width, uint16_t screen_height)
    : active_game(CONSOLE_INTERFACE), screen_width(screen_width), screen_height(screen_height), previous_active_game(active_game), number_of_games(0)
{
    setupIO();

    spi_driver = new SPIDriver(clk, di, -1); // DO NOT REMOVE THIS IT INITIALIZES THE SPI BUS

    gfx = new GL(cs, screen_width, screen_height);

    kb = new Keyboard(gfx);

    Application::set_gfx(gfx);

    Application::set_kb(kb);

    gfx->initGL();
    gfx->clearDisplay();

    console_UI = new ConsoleUI(&active_game);

    console_UI->set_gfx(gfx);

    apps[CONSOLE_INTERFACE] = (Application *)console_UI;

    contextBuffer = gfx->getContext();

    apps[CONSOLE_INTERFACE]->setup();
}

Console::~Console()
{
    console_UI = nullptr;

    for (uint8_t i = 0; i < number_of_games; i++)
        delete apps[i];

    delete kb;

    delete gfx;

    delete spi_driver;
}

void Console::loop()
{
    if (!apps[active_game]->loop())
    {
        if (previous_active_game != 0)
            active_game = 0;
        apps[previous_active_game]->cleanup();
        apps[active_game]->setup();
        previous_active_game = active_game;
    }
}

void Console::set_active_object(Active *object)
{
    Application::set_active_object(object);
}

bool Console::add_application(Application *app)
{
    if (number_of_games == 5)
        return false;

    apps[++number_of_games] = app;

    console_UI->add_name(app->get_name());

    return true;
}

GL *Console::get_gfx() const
{
    return gfx;
}