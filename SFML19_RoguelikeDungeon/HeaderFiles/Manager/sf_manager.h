#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>

#ifndef SF_MANAGER_H
#define SF_MANAGER_H

/**
* A class containing common SFML objects to keep track of.
*/
class SF_Manager {
public:
	const static float DEFAULT_SCREEN_X;
	const static float DEFAULT_SCREEN_Y;

	const static unsigned int MAX_INV_SPELL_SLOTS = 32;
	const static unsigned int NUM_NON_CUR_STATS = 6;
	const static unsigned int TILE = 40;

	static sf::RenderWindow window;
	static sf::View viewUI;
	static sf::View viewWorld;
	static sf::View viewLog;
	static sf::View viewSlots;
	static sf::View viewMap;

	/* min: 0, max(inclusive) : 3
	*/
	static unsigned int worldZoomLevel;

	static sf::Event event;

	static float x;
	static float y;

	static float worldX;
	static float worldY;

	static float slotX;
	static float slotY;
};

#endif