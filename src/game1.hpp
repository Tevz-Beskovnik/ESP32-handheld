#pragma once

#include <Console-IO.hpp>
#include <GL.hpp>

void gameFinish1(GL* gfx)
{
  	#ifdef DEBUG
  	Serial.println("cleaning up!");
  	#endif
}

void gameSetup1(GL* gfx) 
{
	#ifdef DEBUG
	Serial.println("Seting up environment");
	#endif
}

void gameLoop1(GL* gfx)
{
	
}