/**
*
* File: map_screen.h
* Description: Contain the declaration of the Map_Screen class, which represents the map screen.
*/

#ifndef MAP_SCREEN_H
#define MAP_SCREEN_H

#include "screen.h"
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>

class Map_Screen : public Screen {
private:
	static sf::Vector2f oldPos;
	static bool moving;
	static sf::View viewMap;

public:
	Map_Screen();

	bool click_event_handler();

	void mouse_event_handler();

	void draw();
};

#endif