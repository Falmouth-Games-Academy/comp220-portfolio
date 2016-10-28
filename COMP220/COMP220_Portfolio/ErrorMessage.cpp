#include "stdafx.h"
#include "ErrorMessage.h"


void ErrorMessage::initialisationError(const char* message, const char* title)
{
	MessageBoxA(nullptr, message, title, MB_OK | MB_ICONERROR);
}

void ErrorMessage::showErrorMessage(const char* message, const char* title)
{
	// Note: this is specific to Windows, and would need redefining to work on Mac or Linux
	MessageBoxA(nullptr, message, title, MB_OK | MB_ICONERROR);
}