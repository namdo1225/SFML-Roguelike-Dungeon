/**
*
* File: full_rectangle.cpp
* Description: Contain the implementation of the Full_Rectangle class.
* 
*/

#include "Shape/full_rectangle.h"

const sf::Color Full_Rectangle::light_bg[themes] = { sf::Color::White, sf::Color(240, 240, 240) };
const sf::Color Full_Rectangle::dark_bg[themes] = { sf::Color::Black, sf::Color(50, 50, 50) };

const sf::Color Full_Rectangle::light_edge[themes] = { sf::Color(10, 10, 10), sf::Color(70, 70, 70) };
const sf::Color Full_Rectangle::dark_edge[themes] = { sf::Color::White, sf::Color(220, 220, 220) };

const sf::Color Full_Rectangle::light_bg_hover[themes] = { sf::Color(230, 230, 230), sf::Color(220, 220, 220) };
const sf::Color Full_Rectangle::dark_bg_hover[themes] = { sf::Color::White, sf::Color(70, 70, 70) };

const sf::Color Full_Rectangle::light_edge_hover[themes] = { sf::Color(60, 60, 60), sf::Color::Black };
const sf::Color Full_Rectangle::dark_edge_hover[themes] = { sf::Color::Black, sf::Color(240, 240, 240) };

Full_Rectangle::Full_Rectangle() {
	setOutlineThickness(3.f);
}

Full_Rectangle::Full_Rectangle(float x, float y, float w, float h,
	bool hoverable, bool override_theme,
	sf::Color fill, sf::Color outline) : Full_Shape(hoverable, override_theme) {
	if (!override_theme) {
		sf::RectangleShape::setFillColor(Setting_Manager::light ? light_bg[theme] : dark_bg[theme]);
		sf::RectangleShape::setOutlineColor(Setting_Manager::light ? light_edge[theme] : dark_edge[theme]);
	}
	else {
		sf::RectangleShape::setOutlineColor(outline);
		sf::RectangleShape::setFillColor(fill);
	}

	setOutlineThickness(3.f);
	setPosition(x, y);
	setSize(sf::Vector2f(w, h));
}

void Full_Rectangle::flip_theme() {
	sf::Color fill = getFillColor();
	sf::Color outline = getOutlineColor();

	sf::Color theme_fill = Setting_Manager::light ? light_bg[theme] : dark_bg[theme];
	sf::Color theme_outline = Setting_Manager::light ? light_edge[theme] : dark_edge[theme];

	sf::RectangleShape::setFillColor(sf::Color(theme_fill.r, theme_fill.g, theme_fill.g, fill.a ? fill.a : 255));
	sf::RectangleShape::setOutlineColor(sf::Color(theme_outline.r, theme_outline.g, theme_outline.g, outline.a ? outline.a : 255));
}

void Full_Rectangle::highlight(bool hovered) {
	if (hoverable) {
		this->hovered = hovered;
		sf::Color fill = getFillColor();
		sf::Color outline = getOutlineColor();

		sf::Color theme_fill = hovered ? (Setting_Manager::light ? light_bg_hover[theme] : dark_bg_hover[theme]) :
			(Setting_Manager::light ? light_bg[theme] : dark_bg[theme]);
		sf::Color theme_outline = hovered ? (Setting_Manager::light ? light_edge_hover[theme] : dark_edge_hover[theme]) :
			(Setting_Manager::light ? light_edge[theme] : dark_edge[theme]);

		sf::RectangleShape::setFillColor(sf::Color(theme_fill.r, theme_fill.g, theme_fill.g, fill.a ? fill.a : 255));
		sf::RectangleShape::setOutlineColor(sf::Color(theme_outline.r, theme_outline.g, theme_outline.g, outline.a ? outline.a : 255));
	}
}

void Full_Rectangle::setFillColor(const sf::Color& color) {
	if (override_theme)
		sf::RectangleShape::setFillColor(color);
}

void Full_Rectangle::setOutlineColor(const sf::Color& color) {
	if (override_theme)
		sf::RectangleShape::setOutlineColor(color);
}

void Full_Rectangle::setPhysical(float x, float y, float w, float h) {
	if (!override_theme) {
		sf::RectangleShape::setFillColor(Setting_Manager::light ? light_bg[theme] : dark_bg[theme]);
		sf::RectangleShape::setOutlineColor(Setting_Manager::light ? light_edge[theme] : dark_edge[theme]);
	}
	setPosition(x, y);
	setSize(sf::Vector2f(w, h));
}
