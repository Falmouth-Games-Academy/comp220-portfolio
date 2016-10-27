#pragma once
class ErrorMessage
{
public:
	ErrorMessage();
	~ErrorMessage();

	void custom(const char* message, const char* title);
};

