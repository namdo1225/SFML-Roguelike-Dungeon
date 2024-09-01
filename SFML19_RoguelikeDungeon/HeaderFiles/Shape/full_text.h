#include "Manager/font_manager.h"
#include "Manager/sf_manager.h"
#include "Shape/full_shape.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Text.hpp>
#include <functional>

#ifndef FULL_TEXT_H
#define FULL_TEXT_H

/**
* A wrapper class around sf::Text that provide extra functionality like font changing.
*/
class Full_Text : public sf::Text, public Full_Shape, protected Font_Manager, protected SF_Manager {
protected:
	std::function<void()> clickCallback = NULL;

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
	*	hoverable: true if shape is hoverable.
	*	override_theme: true if theme is no longer in effect.
	*	click: optional click callback function.
	*	fill: text's fill color.
	*	outline: text's outline color.
	*/
	Full_Text(float x, float y, float size, const char* text,
		bool hoverable = true, bool override_theme = false, std::function<void()> click = NULL,
		sf::Color fill = sf::Color::Transparent, sf::Color outline = sf::Color::Transparent);

	void changeTheme();

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

	/**
	* Hover over text.
	*/
	void hover();

	/**
	* Call click callback.
	* 
	* Return:
	*	true if callback is called.
	*/
	bool click();

private:
	using sf::Text::setFillColor;
	using sf::Text::setOutlineColor;

	const static sf::Color light[THEMES];
	const static sf::Color dark[THEMES];

	const static sf::Color light_hover[THEMES];
	const static sf::Color dark_hover[THEMES];
};

#endif