#include "Manager/sf_manager.h"
#include "full_rectangle.h"
#include "full_text.h"

#ifndef FULL_TEXTINPUT_H
#define FULL_TEXTINPUT_H

class Full_TextInput : protected SF_Manager {
private:
	bool focused = false;
	unsigned int maxLength = 20;

public:
	Full_Rectangle rect;
	Full_Text text;

	Full_TextInput(const char* defaultText, unsigned int length, float x, float y, float w, float h, float fontSize = 0.f, float fontOutline = 0.f);

	void click();

	void handleTextEvent();
};

#endif