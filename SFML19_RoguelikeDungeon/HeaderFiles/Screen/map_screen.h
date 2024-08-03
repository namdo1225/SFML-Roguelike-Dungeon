/**
*
* File: map_screen.h
* Description: Contain the declaration of the Map_Screen class, which represents the map screen.
*/

#ifndef MAP_SCREEN_H
#define MAP_SCREEN_H

#include "screen.h"

class Map_Screen : public Screen {
private:
	static sf::Vector2f oldPos;
	static bool moving;
	static sf::View viewMap;

public:
	Map_Screen();

	virtual void click_event_handler();

	virtual void mouse_event_handler();

	virtual void draw();
};

#endif