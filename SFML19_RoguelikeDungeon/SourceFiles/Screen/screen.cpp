#include "Screen/screen.h"

Display Screen::display = Title;
sf::RenderWindow Screen::window;

Screen::Screen(unsigned int txt, unsigned int rect, sf::Font& font) {
	while (txt > 0) {
		texts.push_back(sf::Text());
		unsigned int i = texts.size() - 1;
		texts[i].setFont(font);
		texts[i].setFillColor(sf::Color::White);
		texts[i].setCharacterSize(24);
		texts[i].setStyle(sf::Text::Bold);
		txt--;
	}

	while (rect > 0) {
		rects.push_back(sf::RectangleShape());
		unsigned int i = rects.size() - 1;
		rects[i].setFillColor(sf::Color::Black);
		rects[i].setOutlineThickness(3.f);
		rects[i].setOutlineColor(sf::Color::White);
		rect--;
	}
}

void Screen::draw(sf::RenderWindow& window) {
	for (sf::RectangleShape rect : rects)
		window.draw(rect);
	for (sf::Text text : texts)
		window.draw(text);
}

void Screen::setup_helper(bool element, unsigned int i, const char* text, int x, int y, int sx, int sy) {
	if (element) {
		rects[i].setPosition(x, y);
		rects[i].setSize(sf::Vector2f(sx, sy));
	}
	else {
		texts[i].setPosition(x, y);
		texts[i].setString(text);
	}
}

bool Screen::mouse_in_helper(bool element, unsigned int i, int x, int y) {
	return (element && rects[i].getGlobalBounds().contains(sf::Vector2f(x, y))) ||
		(!element && texts[i].getGlobalBounds().contains(sf::Vector2f(x, y)));
}