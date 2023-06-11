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
		char test[10];
		uint8_t len = 10;
		kb->prompt_string(test, len);

		Serial.println("Got char:");
		Serial.printf("Read character from keyboard: ");
		Serial.println(test);

		return false;
	}

	void cleanup() override
	{
#ifdef DEBUG
		Serial.println("cleaning up!");
#endif
	}
};