/**
*
* File: full_text.h
* Description: Contain the declaration of the Full_Text class,
* which is a wrapper class around sf::Text that provide extra functionality.
*
*/

#ifndef FULL_TEXT_H
#define FULL_TEXT_H

#include "..\SFML-2.5.1/include/SFML/Graphics.hpp"

class Full_Text : public sf::Text {
private:
	sf::Color light = sf::Color::Black;
	sf::Color dark = sf::Color::White;

public:
};

#endif