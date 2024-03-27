/**
*
* File: full_text.cpp
* Description: Contain the implementation of the Full_Text class.
*
*/

#include "Shape/full_text.h"
#include "Manager/font_manager.h"

const sf::Color Full_Text::light[Full_Shape::themes] = { sf::Color(87, 87, 87), sf::Color(100, 100, 100) };
const sf::Color Full_Text::dark[Full_Shape::themes] = { sf::Color::White, sf::Color(175, 175, 175) };

const sf::Color Full_Text::light_hover[Full_Shape::themes] = { sf::Color::Black, sf::Color(200, 117, 117) };
const sf::Color Full_Text::dark_hover[Full_Shape::themes] = { sf::Color(50, 50, 50), sf::Color::White};

Full_Text::Full_Text() {
	sf::Text::setOutlineColor(sf::Color::Black);
	setFont(Font_Manager::get_selected());
}

Full_Text::Full_Text(float x, float y, float size, const char* text,
	unsigned int theme, bool hoverable, bool is_light, bool override_theme,
	sf::Color fill, sf::Color outline): Full_Shape(theme, hoverable, is_light, override_theme) {
	if (!override_theme) {
		sf::Text::setFillColor(dark_mode ? dark[theme] : light[theme]);
	}
	else {
		sf::Text::setOutlineColor(outline);
		sf::Text::setFillColor(fill);
	}

	setPosition(x, y);
	setCharacterSize(size);
	setString(text);
	setFont(Font_Manager::get_selected());
}

void Full_Text::flip_theme(bool light_mode, unsigned int new_theme) {
	dark_mode = !light_mode;
	theme = new_theme;
	sf::Color fill = getFillColor();
	sf::Color theme_fill = dark_mode ? dark[theme] : light[theme];
	sf::Text::setFillColor(sf::Color(theme_fill.r, theme_fill.g, theme_fill.g, fill.a ? fill.a : 255));
}

void Full_Text::highlight(bool hovered) {
	if (hoverable) {
		this->hovered = hovered;
		sf::Color fill = getFillColor();
		sf::Color theme_fill = hovered ? (dark_mode ? dark_hover[theme] : light_hover[theme]) :
			(dark_mode ? dark[theme] : light[theme]);
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
