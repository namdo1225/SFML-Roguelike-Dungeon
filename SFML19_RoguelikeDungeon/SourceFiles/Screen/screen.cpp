#include "Screen/screen.h"

Screen::Screen(unsigned int txt, unsigned int rect, sf::Font& font)
{
	while (txt > 0) {
		texts.push_back(sf::Text());
		unsigned int i = texts.size() - 1;
		texts[i].setFont(font);
		texts[i].setFillColor(sf::Color::White);
		texts[i].setCharacterSize(24);
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