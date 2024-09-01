#include "full_rectangle.h"
#include "full_text.h"
#include "Manager/sf_manager.h"

#ifndef FULL_TEXTINPUT_H
#define FULL_TEXTINPUT_H

/**
* Validation enum to filter input.
*/
enum InputValidation {
	// Accepts all characters.
	AllValidation,
	// Accepts alphanumeric characters.
	AlphanumericValidation,
	// filters input to only accept letters from the alphabet.
	AlphabetValidation,
	// Accepts only numbers.
	NumberValidation,
	// Accepts 0 or 1.
	TrueFalseValidation,
	// Alphanumeric with space acceptance.
	AlphanumspaceValidation,
};

/**
* Class that provides a text input functionality with basic input validation.
*/
class Full_TextInput : protected SF_Manager {
private:
	bool focused = false;
	unsigned int maxLength = 20;
	InputValidation validation = AllValidation;
	static Full_TextInput* chosenInput;

public:
	Full_Rectangle rect;
	Full_Text text;

	/*
	* Constructor for Full_Textbox.
	*
	* Parameter:
	*	defaultText: initial default text.
	*	length: maximum length of the text input.
	*	x: text input's x position.
	*	y: text input's y position.
	*	w: text input's width.
	*	h: text input's height.
	*	validation: text input's validation type.
	*	fontSize: optional font size.
	*	fontOutline: optional font outline.
	*/
	Full_TextInput(const char* defaultText, unsigned int length, float x, float y, float w, float h, InputValidation validation, float fontSize = 0.f, float fontOutline = 0.f);

	/**
	* Manages the focus of the text input
	*
	* Return:
	*	true if this text input is focused on.
	*/
	bool click();

	/**
	* Handles character being typed in.
	*/
	void handleTextEvent();

	/*
	* Clears text input.
	*/
	void clear();
};

#endif