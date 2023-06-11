#pragma once

#include <Application.hpp>

class Game1 : public Application
{
public:
	Game1()
		: Application("Game 1")
	{
		;
	}

	void setup() override
	{
#ifdef DEBUG
		Serial.println("Seting up environment");
#endif
	}

	bool loop() override
	{
		return false;
	}

	void cleanup() override
	{
#ifdef DEBUG
		Serial.println("cleaning up!");
#endif
	}
};