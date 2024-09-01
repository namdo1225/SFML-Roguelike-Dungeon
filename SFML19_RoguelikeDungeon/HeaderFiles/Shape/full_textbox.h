#include "Shape/full_rectangle.h"
#include "Manager/sf_manager.h"
#include "Shape/full_text.h"
#include <functional>

#ifndef FULL_TEXTBOX_H
#define FULL_TEXTBOX_H

/**
* A class that represents a rectangle with a text. Supports centering text inside rectangle
* and click callback function.
*/
class Full_Textbox : protected SF_Manager {
protected:
	std::function<void()> callback = NULL;

public:
	Full_Rectangle rect;
	Full_Text text;

	/*
	* Constructor for Full_Textbox.
	*
	* Parameter:
	*	text: text's string.
	*	x: textbox's x position.
	*	y: textbox's y position.
	*	w: textbox's width.
	*	h: textbox's height.
	*	func: click callback function.
	*	fontSize: optional font size.
	*	fontOutline: optional font outline.
	*/
	Full_Textbox(const char* text, float x, float y, float w, float h, std::function<void()> func, float fontSize = 0.f, float fontOutline = 0.f);

	/**
	* Hover over textbox.
	*/
	void hover();

	/**
	* Call click callback.
	*
	* Return:
	*	true if callback is called.
	*/
	bool click();

	/**
	* Updates the callback.
	* 
	* Parameter:
	*	func: new callback function.
	*/
	void updateCallback(std::function<void()> func);

	/**
	* Centers the text to the rectangle.
	*/
	void recenterText();
};

#endif