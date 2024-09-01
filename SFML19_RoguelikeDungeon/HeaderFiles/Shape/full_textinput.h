#include "full_rectangle.h"
#include "full_text.h"
#include "Manager/sf_manager.h"

#ifndef FULL_TEXTINPUT_H
#define FULL_TEXTINPUT_H

enum InputValidation {
	AllValidation,
	AlphanumericValidation,
	AlphabetValidation,
	NumberValidation,
	TrueFalseValidation,
	AlphanumspaceValidation,
};

class Full_TextInput : protected SF_Manager {
private:
	bool focused = false;
	unsigned int maxLength = 20;
	InputValidation validation = AllValidation;
	static Full_TextInput* chosenInput;

public:
	Full_Rectangle rect;
	Full_Text text;

	Full_TextInput(const char* defaultText, unsigned int length, float x, float y, float w, float h, InputValidation validation, float fontSize = 0.f, float fontOutline = 0.f);

	bool click();

	void handleTextEvent();

	void clear();
};

#endif