#include "stdafx.h"
#include "ErrorMessage.h"


ErrorMessage::ErrorMessage()
{
}

ErrorMessage::~ErrorMessage()
{
}

//For use when another more specific error message is not available
void ErrorMessage::custom(const char * message, const char * title)
{
	MessageBoxA(nullptr, message, title, MB_OK | MB_ICONERROR);
}

//For specific error messages that require the program to protect the os
void ErrorMessage::quitCustom(const char * message, const char * title)
{
	SDL_Quit();
	MessageBoxA(nullptr, message, title, MB_OK | MB_ICONERROR);
}
