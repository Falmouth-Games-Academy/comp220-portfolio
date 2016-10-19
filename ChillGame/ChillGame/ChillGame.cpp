// ChillGame.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ErrorMessage.h"

int main(int argc, char* args[])
{
	ErrorMessage errorMessage;

	errorMessage.custom("Oh dear!", "No content yet.");

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Quit();

    return 0;
}

