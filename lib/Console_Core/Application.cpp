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

void Application::set_active_object(Active *object)
{
    Application::object = object;
}

[[nodiscard]] const char *Application::get_name() const
{
    return name;
}