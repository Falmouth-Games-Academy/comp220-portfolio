#include "stdafx.h"
#include "ErrorMessage.h"

//Error messages are for windows and not mac or linux


ErrorMessage::ErrorMessage()
{
}


ErrorMessage::~ErrorMessage()
{
}

void ErrorMessage::custom(const char * message, const char * title)
{
	MessageBoxA(nullptr, message, title, MB_OK | MB_ICONERROR);
}

//TODO add sdl error add opengl error