/**
*
* File: screen.h
* Description: Contain the declaration of the Screen class, which represents a game scene.
*/

#ifndef SCREEN_H
#define SCREEN_H

#include "..\SFML-2.5.1/include/SFML/Graphics.hpp"
#include <iostream>
#include <array>

enum Display { Title, Exit, Name, Stat, Level, Game, Menu, Inventory, Spell, Map, Scan, Help };
//enum Command { Title, Exit, Name, Stat, Level, Game, Menu, Inventory, Spell, Map, Scan, Help };

class Screen {
protected:
	std::vector<sf::Text> texts;
	std::vector<sf::RectangleShape> rects;
	static Display display;

	static sf::RenderWindow window;

	void setup_helper(bool element, unsigned int i, const char* text, int x, int y, int sx, int sy);
	bool mouse_in_helper(bool element, unsigned int i, int x, int y);


public:
	static const unsigned int num_screens = 12;
	static std::array<bool, num_screens> visibilities;
	static std::array<std::unique_ptr<Screen>, num_screens> screens;

	/**
	* Constructor for Screen.
	*/
	Screen(unsigned int txt, unsigned int rect, sf::Font& font);

	/**
	* Draw the screen.
	*
	* Parameter:
	*	window: a sf::RenderWindow object where the room will be drawn on.
	*/
	void draw(sf::RenderWindow& window);

	/**
	* Event handler for the screen.
	*
	* Parameter:
	*	x: an int for the mouse cursor's x position.
	* 	y: an int for the mouse cursor's y position.
	*/
	//virtual void event_handler(int x, int y);
};


#endif