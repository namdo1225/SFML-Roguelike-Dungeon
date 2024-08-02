#include "Shape/full_rectangle.h"
#include "Shape/fulL_text.h"
#include "Manager/sf_manager.h"

#ifndef FULL_TEXTBOX_H
#define FULL_TEXTBOX_H

class Full_Textbox : public SF_Manager {
public:
	Full_Rectangle rect;
	Full_Text text;
	void (*callback)();

	Full_Textbox(const char* text, float x, float y, float w, float h, void (*func)(), float fontSize = 0.f, float fontOutline = 0.f);

	void hover();

	void click();
};

#endif