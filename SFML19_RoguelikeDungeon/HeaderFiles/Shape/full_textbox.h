#include "Shape/full_rectangle.h"
#include "Manager/sf_manager.h"
#include "Shape/full_text.h"
#include <functional>

#ifndef FULL_TEXTBOX_H
#define FULL_TEXTBOX_H

class Full_Textbox : protected SF_Manager {
protected:
	std::function<void()> callback = NULL;

public:
	Full_Rectangle rect;
	Full_Text text;

	Full_Textbox(const char* text, float x, float y, float w, float h, std::function<void()> func, float fontSize = 0.f, float fontOutline = 0.f);

	void hover();

	bool click();

	void updateCallback(std::function<void()> func);

	void recenterText();
};

#endif