#include "Shape/full_rectangle.h"
#include "Manager/sf_manager.h"
#include "Shape/full_text.h"

#ifndef FULL_TEXTBOX_H
#define FULL_TEXTBOX_H

class Full_Textbox : protected SF_Manager {
protected:
	void (*callback)() = NULL;
public:
	Full_Rectangle rect;
	Full_Text text;

	Full_Textbox(const char* text, float x, float y, float w, float h, void (*func)(), float fontSize = 0.f, float fontOutline = 0.f);

	void hover();

	void click();
};

#endif