/**
*
* File: full_shape.h
* Description: Contain the declaration of the Full_Shape to allow children
* wrapper classes to maintain a common interface.
*
*/

#ifndef FULL_SHAPE_H
#define FULL_SHAPE_H

#include "..\SFML-2.5.1/include/SFML/Graphics.hpp"

class Full_Shape {
protected:
	const static unsigned int themes = 2;
	bool dark_mode = true;
	bool hovered = false;
	bool hoverable = true;
	bool override_theme = false;
	unsigned int theme = 0;

public:

	virtual void flip_theme(bool light, unsigned int new_theme) = 0;

	virtual void highlight(bool hovered = true) = 0;

	virtual void setFillColor(const sf::Color& color) = 0;

	virtual void setOutlineColor(const sf::Color& color) = 0;

	Full_Shape(unsigned int theme = 0, bool hoverable = true, bool is_light = false, bool override_theme = false);

	void setThemeAndHover(unsigned int theme = 0, bool hoverable = true, bool is_light = false, bool override_theme = false);

	bool getHover();
};

#endif