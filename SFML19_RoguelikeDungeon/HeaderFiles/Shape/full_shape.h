#include "Manager/setting_manager.h"
#include <SFML/Graphics/Color.hpp>

#ifndef FULL_SHAPE_H
#define FULL_SHAPE_H

/**
* A wrapper, parent classes to maintain a common interface for
* all "Full" subclasses.
*/
class Full_Shape : protected Setting_Manager {
protected:
	bool hovered = false;
	bool hoverable = true;
	bool override_theme = false;

public:

	/*
	* Constructor for Full_Shape.
	*
	* Parameter:
	*	hoverable: true if shape is hoverable.
	*	override_theme: true if theme is no longer in effect.
	*/
	Full_Shape(bool hoverable = true, bool override_theme = false);

	/*
	* Change shape's theme.
	*/
	virtual void changeTheme() = 0;

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
	*	color: The new fill's color.
	*/
	virtual void setFillColor(const sf::Color& color) = 0;

	/*
	* Sets shape's outline color.
	*
	* Parameter:
	*	color: The new outline's color.
	*/
	virtual void setOutlineColor(const sf::Color& color) = 0;

	/*
	* Sets many configs for the shape.
	*
	* Parameter:
	*	hoverable: true if shape is hoverable.
	*	override_theme: true if theme is no longer in effect.
	*/
	void setThemeAndHover(bool hoverable = true, bool override_theme = false);

	/*
	* Getters for hovered.
	*
	* Return:
	*	hovered's value.
	*/
	bool getHover();
};

#endif