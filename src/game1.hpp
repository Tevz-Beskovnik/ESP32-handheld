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
		char got = kb->prompt();

		Serial.println("Got char:");
		Serial.printf("Read character from keyboard: %c\n", got);

		return false;
	}

	void cleanup() override
	{
#ifdef DEBUG
		Serial.println("cleaning up!");
#endif
	}
};