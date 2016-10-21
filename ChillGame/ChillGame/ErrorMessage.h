#pragma once

//Windows Only
//Error messages are not available for mac or linux.

class ErrorMessage
{
public:
	ErrorMessage();
	~ErrorMessage();

	void custom(const char* message, const char* title);

	void quitCustom(const char* message, const char* title);


};

