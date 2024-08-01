/**
*
* File: full_text.cpp
* Description: Contain the implementation of the Full_Text class.
*
*/

#include "Shape/full_text.h"
#include "Manager/font_manager.h"

const sf::Color Full_Text::light[themes] = { sf::Color(87, 87, 87), sf::Color(100, 100, 100) };
const sf::Color Full_Text::dark[themes] = { sf::Color(230, 230, 230), sf::Color(175, 175, 175)};

const sf::Color Full_Text::light_hover[themes] = { sf::Color::Black, sf::Color(200, 117, 117) };
const sf::Color Full_Text::dark_hover[themes] = { sf::Color(50, 50, 50), sf::Color::White};

Full_Text::Full_Text() {
	sf::Text::setOutlineColor(sf::Color::Black);
	setFont(get_selected());
}

Full_Text::Full_Text(float x, float y, float size, const char* text,
	bool hoverable, bool override_theme,
	sf::Color fill, sf::Color outline): Full_Shape(hoverable, override_theme) {
	if (!override_theme) {
		sf::Text::setFillColor(Setting_Manager::light ? light[theme] : dark[theme]);
	}
	else {
		sf::Text::setOutlineColor(outline);
		sf::Text::setFillColor(fill);
	}

	setPosition(x, y);
	setCharacterSize(size);
	setString(text);
	setFont(get_selected());
}

void Full_Text::flip_theme() {
	sf::Color fill = getFillColor();
	sf::Color theme_fill = Setting_Manager::light ? light[theme] : dark[theme];
	sf::Text::setFillColor(sf::Color(theme_fill.r, theme_fill.g, theme_fill.g, fill.a ? fill.a : 255));
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
