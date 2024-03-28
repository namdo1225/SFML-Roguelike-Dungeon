/**
*
* File: full_shape.cpp
* Description: Contain the implementation of the Full_Shape class.
*
*/

#include "Shape/full_shape.h"

const unsigned int Full_Shape::themes;

Full_Shape::Full_Shape(unsigned int theme,
	bool hoverable, bool is_light, bool override_theme):
	theme(theme), hoverable(hoverable), override_theme(override_theme), dark_mode(!is_light) {
}

void Full_Shape::setThemeAndHover(unsigned int theme,
	bool hoverable, bool is_light, bool override_theme) {
	this->theme = theme;
	this->hoverable = hoverable;
	this->override_theme = override_theme;
	this->dark_mode = !is_light;
}

bool Full_Shape::getHover() {
	return hovered;
}
