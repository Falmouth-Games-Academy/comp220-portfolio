#include "stdafx.h"
#include "ErrorMessage.h"

void ErrorMessage::createErrorMessage(const char* message, const char* title)
{
	MessageBoxA(nullptr, message, title, MB_OK | MB_ICONERROR);
}