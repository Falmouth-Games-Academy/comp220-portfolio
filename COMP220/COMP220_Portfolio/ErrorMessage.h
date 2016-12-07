#pragma once
//! Error class
/*!
	contains functions to create errors 
*/
class ErrorMessage : std::exception
{
public:
	//! Error message function
	/*!
	Creates an error pop up when called
	*/
	void createErrorMessage(const char* message, const char* title);
};
