#include "screen.h"
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>

#ifndef MAP_SCREEN_H
#define MAP_SCREEN_H

/**
* Represents the map screen.
*/
class Map_Screen : public Screen {
private:
	static sf::Vector2f oldPos;
	static bool moving;
	static sf::View viewMap;

public:
	Map_Screen();

	bool handleClickEvent();

	void handleMouseEvent();

	void draw();
};

#endif