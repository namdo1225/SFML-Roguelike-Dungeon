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

	/*
	* Constructor for Full_Shape.
	*
	* Parameter:
	*	theme: the theme number.
	*	hoverable: true if shape is hoverable.
	*	is_light: true if light mode.
	*	override_theme: true if theme is no longer in effect.
	*/
	Full_Shape(unsigned int theme = 0, bool hoverable = true, bool is_light = false, bool override_theme = false);

	/*
	* Flip shape's light mode and theme.
	* 
	* Parameter:
	*	light: true if light mode.
	*	new_theme: the theme number.
	*/
	virtual void flip_theme(bool light, unsigned int new_theme) = 0;

	/*
	* Highlights shape.
	*
	* Parameter:
	*	hovered: true if shape is hovered over.
	*/
	virtual void highlight(bool hovered = true) = 0;

	/*
	* Sets shape's fill color.
	*
	* Parameter:
	*	color: a const sf::Color& for the new fill's color.
	*/
	virtual void setFillColor(const sf::Color& color) = 0;

	/*
	* Sets shape's outline color.
	*
	* Parameter:
	*	color: a const sf::Color& for the new outline's color.
	*/
	virtual void setOutlineColor(const sf::Color& color) = 0;

	/*
	* Sets many configs for the shape.
	*
	* Parameter:
	*	theme: the theme number.
	*	hoverable: true if shape is hoverable.
	*	is_light: true if light mode.
	*	override_theme: true if theme is no longer in effect.
	*/
	void setThemeAndHover(unsigned int theme = 0, bool hoverable = true, bool is_light = false, bool override_theme = false);

	/*
	* Getters for hovered.
	*
	* Return:
	*	hovered's value.
	*/
	bool getHover();
};

#endif