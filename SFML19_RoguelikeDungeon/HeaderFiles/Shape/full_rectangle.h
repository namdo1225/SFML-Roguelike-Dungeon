/**
*
* File: full_rectangle.h
* Description: Contain the declaration of the Full_Rectangle class,
* which is a wrapper class around sf::RectangleShape that provide extra functionality.
*
*/

#ifndef FULL_RECT_H
#define FULL_RECT_H

#include "..\SFML-2.5.1/include/SFML/Graphics.hpp"

class Full_Rectangle : public sf::RectangleShape {
private:
	sf::Color light_bg = sf::Color::White;
	sf::Color dark_bg = sf::Color::Black;

	sf::Color light_border = sf::Color::Black;
	sf::Color dark_border = sf::Color::White;

	bool dark_mode = true;
	bool hovered = false;
	bool detectHover = false;

public:

	Full_Rectangle();

	void flip_theme();

	void highlight();
};

#endif