#include <Application.hpp>

GL *Application::gfx = nullptr;

Keyboard *Application::kb = nullptr;

Application::Application(const char *name)
    : name(name)
{
    ;
}

void Application::set_gfx(GL *gfx)
{
    Application::gfx = gfx;
}

void Application::set_kb(Keyboard *kb)
{
    Application::kb = kb;
}

[[nodiscard]] const char *Application::get_name() const
{
    return name;
}