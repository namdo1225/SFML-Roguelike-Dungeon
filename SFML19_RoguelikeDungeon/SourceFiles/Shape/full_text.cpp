/**
*
* File: full_text.cpp
* Description: Contain the implementation of the Full_Text class.
*
*/

#include "Shape/full_text.h"
#include <functional>
#include <Manager/font_manager.h>
#include <Manager/setting_manager.h>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <Shape/full_shape.h>

const sf::Color Full_Text::light[themes] = { sf::Color(87, 87, 87), sf::Color(100, 100, 100) };
const sf::Color Full_Text::dark[themes] = { sf::Color(230, 230, 230), sf::Color(175, 175, 175)};

const sf::Color Full_Text::light_hover[themes] = { sf::Color::Black, sf::Color(200, 117, 117) };
const sf::Color Full_Text::dark_hover[themes] = { sf::Color(50, 50, 50), sf::Color::White};

Full_Text::Full_Text() {
	sf::Text::setOutlineColor(sf::Color::Black);
	setFont(get_selected());
	setCharacterSize(24.f);
	setStyle(sf::Text::Bold);
}

Full_Text::Full_Text(float x, float y, float size, const char* text,
	bool hoverable, bool override_theme, std::function<void()> click,
	sf::Color fill, sf::Color outline): Full_Shape(hoverable, override_theme) {
	if (!override_theme) {
		sf::Color fill = getFillColor();
		sf::Color theme_fill = Setting_Manager::light ? light[theme] : dark[theme];
		sf::Text::setFillColor(sf::Color(theme_fill.r, theme_fill.g, theme_fill.g, fill.a ? fill.a : 255));
	}
	else {
		sf::Text::setOutlineColor(outline);
		sf::Text::setFillColor(fill);
	}

	setPosition(x, y);

	setCharacterSize(size > 0.f ? size : 24.f);
	setString(text);
	setFont(get_selected());
	setStyle(sf::Text::Bold);

	clickCallback = click;
}

void Full_Text::flip_theme() {
	sf::Color fill = getFillColor();
	sf::Color theme_fill = Setting_Manager::light ? light[theme] : dark[theme];
	sf::Text::setFillColor(sf::Color(theme_fill.r, theme_fill.g, theme_fill.g, fill.a ? fill.a : 255));
	setFont(Font_Manager::get_selected());
}

void Full_Text::highlight(bool hovered) {
	if (hoverable) {
		this->hovered = hovered;
		sf::Color fill = getFillColor();
		sf::Color theme_fill = hovered ? (Setting_Manager::light ? light_hover[theme] : dark_hover[theme]) :
			(Setting_Manager::light ? light[theme] : dark[theme]);
		sf::Text::setFillColor(sf::Color(theme_fill.r, theme_fill.g, theme_fill.g, fill.a ? fill.a : 255));
	}
}

void Full_Text::setFillColor(const sf::Color& color) {
	if (override_theme)
		sf::Text::setFillColor(color);
}

void Full_Text::setOutlineColor(const sf::Color& color) {
	if (override_theme)
		sf::Text::setOutlineColor(color);
}

void Full_Text::setPhysical(float x, float y, const char* text, float size, float outline) {
	setPosition(x, y);
	setString(text);
	setOutlineThickness(outline);
	if (size != NULL)
		setCharacterSize(size);
}

void Full_Text::hover() {
	bool in = getGlobalBounds().contains(sf::Vector2f(x, y));
	if (in && !hovered)
		highlight();
	else if (!in && hovered)
		highlight(false);
}

bool Full_Text::click() {
	if (getGlobalBounds().contains(sf::Vector2f(x, y)) && clickCallback != NULL) {
		clickCallback();
		return true;
	}
	return false;
}
