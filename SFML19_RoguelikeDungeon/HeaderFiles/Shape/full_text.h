/**
*
* File: full_text.h
* Description: Contain the declaration of the Full_Text class,
* which is a wrapper class around sf::Text that provide extra functionality.
*
*/

#ifndef FULL_TEXT_H
#define FULL_TEXT_H

#include "..\SFML-2.6.1/include/SFML/Graphics.hpp"
#include "Shape/full_shape.h"

class Full_Text : public sf::Text, public Full_Shape {
public:

	/*
	* Constructor for Full_Text.
	*/
	Full_Text();

	/*
	* Constructor for Full_Text.
	*
	* Parameter:
	*	x: text's x position.
	*	y: text's y position.
	*	size: text's font size.
	*	text: text's string.
	*	theme: the theme number.
	*	hoverable: true if shape is hoverable.
	*	is_light: true if light mode.
	*	override_theme: true if theme is no longer in effect.
	*	outline: text's outline color.
	*	fill: text's fill color.
	*/
	Full_Text(float x, float y, float size, const char* text,
		unsigned int theme = 0, bool hoverable = true, bool is_light = false, bool override_theme = false, 
		sf::Color fill = sf::Color::Transparent, sf::Color outline = sf::Color::Transparent);

	void flip_theme(bool light_mode, unsigned int new_theme);

	void highlight(bool hovered = true);

	void setFillColor(const sf::Color& color);

	void setOutlineColor(const sf::Color& color);

	/*
	* Sets text's physical descriptions.
	*
	* Parameter:
	*	x: text's x position.
	*	y: text's y position.
	*	text: text's string.
	*	size: text's font size.
	*	outline: text's outline size.
	*/
	void setPhysical(float x, float y, const char* text, float size, float outline);

private:
	using sf::Text::setFillColor;
	using sf::Text::setOutlineColor;

	const static sf::Color light[themes];
	const static sf::Color dark[themes];

	const static sf::Color light_hover[themes];
	const static sf::Color dark_hover[themes];
};

#endif