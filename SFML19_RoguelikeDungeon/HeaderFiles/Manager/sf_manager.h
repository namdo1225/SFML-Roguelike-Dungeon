/**
*
* File: sf_manager.h
* Description: Contain the declaration of the SF_Manager class, which represents a parent class
* containing common SFML objects to keep track of.
*
*/

#ifndef SF_MANAGER_H
#define SF_MANAGER_H

#include "..\SFML-2.5.1/include/SFML/Graphics.hpp"
#include "Shape/full_text.h"

class SF_Manager {
protected:
	const static unsigned int LOG_SIZE = 50;

	static std::vector<Full_Text> logs;

	static void log_view(bool screen = false);

public:
	const static unsigned int MAX_INV_SPELL_SLOTS = 32;
	const static unsigned int NUM_NON_CUR_STATS = 6;

	static sf::RenderWindow window;
	static sf::View viewUI;
	static sf::View viewWorld;
	static sf::View viewLog;

	static sf::Event event;
	static sf::Vector2i mouse_pos;
	static sf::Vector2f mouse_world;

	static float x;
	static float y;

	static float world_x;
	static float world_y;

	static void log_add(const char* log);
};

#endif