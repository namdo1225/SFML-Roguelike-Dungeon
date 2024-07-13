/**
*
* File: full_rectangle.h
* Description: Contain the declaration of the Full_Rectangle class,
* which is a wrapper class around sf::RectangleShape that provide extra functionality.
*
*/

#ifndef FULL_RECT_H
#define FULL_RECT_H

#include "Shape/full_shape.h"
#include "..\SFML-2.6.1/include/SFML/Graphics.hpp"

class Full_Rectangle : public sf::RectangleShape, public Full_Shape {
public:

	/*
	* Constructor for Full_Rectangle.
	*/
	Full_Rectangle();

	/*
	* Constructor for Full_Rectangle.
	* 
	* Parameter:
	*	x: rectangle's x position.
	*	y: rectangle's y position.
	*	w: rectangle's width.
	*	h: rectangle's height.
	*	theme: the theme number.
	*	hoverable: true if shape is hoverable.
	*	is_light: true if light mode.
	*	override_theme: true if theme is no longer in effect.
	*	outline: rectangle's outline color.
	*	fill: rectangle's fill color.
	*/
	Full_Rectangle(float x, float y, float w, float h,
		unsigned int theme = 0, bool hoverable = true, bool is_light = false, bool override_theme = false,
		sf::Color fill = sf::Color::Transparent, sf::Color outline = sf::Color::Transparent);

	void flip_theme(bool light, unsigned int new_theme);

	void highlight(bool hovered = true);

	void setFillColor(const sf::Color& color);

	void setOutlineColor(const sf::Color& color);

	/*
	* Sets shape's physical descriptions.
	*
	* Parameter:
	*	x: rectangle's x position.
	*	y: rectangle's y position.
	*	w: rectangle's width.
	*	h: rectangle's height.
	*/
	void setPhysical(float x, float y, float w, float h);

private:
	using sf::RectangleShape::setFillColor;
	using sf::RectangleShape::setOutlineColor;

	const static sf::Color light_bg[themes];
	const static sf::Color dark_bg[themes];

	const static sf::Color light_edge[themes];
	const static sf::Color dark_edge[themes];

	const static sf::Color light_bg_hover[themes];
	const static sf::Color dark_bg_hover[themes];

	const static sf::Color light_edge_hover[themes];
	const static sf::Color dark_edge_hover[themes];

};

#endif