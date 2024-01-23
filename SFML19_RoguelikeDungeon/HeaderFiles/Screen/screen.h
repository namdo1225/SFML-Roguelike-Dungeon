/**
*
* File: screen.h
* Description: Contain the declaration of the Screen class, which represents a game scene.
*/

#ifndef SCREEN_H
#define SCREEN_H

#include "..\SFML-2.5.1/include/SFML/Graphics.hpp"
#include <array>

class Screen {
private:
	std::vector<sf::Text> texts;
	std::vector<sf::RectangleShape> rects;

public:
	static const unsigned int screens = 12;
	static std::array<bool, screens> visibilities;
	static std::shared_ptr<Screen> active;

	/**
	* Constructor for Screen.
	*/
	Screen(unsigned int txt, unsigned int rect, sf::Font& font);

	/**
	* Method to load graphics.
	*/
	virtual void load();

	void draw(sf::RenderWindow& window);

	virtual void event_handler(int x, int y);
};


#endif