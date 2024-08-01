/**
*
* File: full_shape.cpp
* Description: Contain the implementation of the Full_Shape class.
*
*/

#include "Shape/full_shape.h"

Full_Shape::Full_Shape(
	bool hoverable, bool override_theme): hoverable(hoverable), override_theme(override_theme) {}

void Full_Shape::setThemeAndHover(
	bool hoverable, bool override_theme) {
	this->hoverable = hoverable;
	this->override_theme = override_theme;
}

bool Full_Shape::getHover() {
	return hovered;
}