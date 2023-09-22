#pragma once

#include <Keyboard.hpp>
#include <Active-Object.hpp>
#include <GL.hpp>

class Application
{
public:
    Application(const char *name);

    static void set_gfx(GL *gfx);

    static void set_kb(Keyboard *kb);

    static void set_active_object(Active *object);

    [[nodiscard]] const char *get_name() const;

    virtual void setup() = 0;

    virtual bool loop() = 0;

    virtual void cleanup() = 0;

protected:
    const char *name;

    static GL *gfx;

    static Keyboard *kb;

    static Active *object;
};
