/**
*
* File: sf_manager.h
* Description: Contain the declaration of the SF_Manager class, which represents a parent class
* containing common SFML objects to keep track of.
*
*/

#ifndef SF_MANAGER_H
#define SF_MANAGER_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

class SF_Manager {
public:
	const static float DEFAULT_SCREEN_X;
	const static float DEFAULT_SCREEN_Y;

	const static unsigned int MAX_INV_SPELL_SLOTS = 32;
	const static unsigned int NUM_NON_CUR_STATS = 6;

	static sf::RenderWindow window;
	static sf::View viewUI;
	static sf::View viewWorld;
	static sf::View viewLog;

	/* min: 0, max(inclusive) : 3
	*/
	static unsigned int worldZoomLevel;

	static sf::Event event;
	static sf::Vector2i mouse_pos;
	static sf::Vector2f mouse_world;

	static float x;
	static float y;

	static float world_x;
	static float world_y;
};

#endif