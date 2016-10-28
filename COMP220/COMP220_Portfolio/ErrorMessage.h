#pragma once
class ErrorMessage : std::exception
{
public:

	void initialisationError(const char* message, const char* title);

	void showErrorMessage(const char* message, const char* title);

private:
	std::string message;
};
