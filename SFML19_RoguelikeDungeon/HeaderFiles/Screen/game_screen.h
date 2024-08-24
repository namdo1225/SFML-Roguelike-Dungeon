/**
*
* File: game_screen.h
* Description: Contain the declaration of the Game_Screen class, which represents the main gameplay screen.
*/

#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include "Candle/LightingArea.hpp"
#include "Candle/RadialLight.hpp"
#include "screen.h"
#include <SFML/System/Clock.hpp>
#include <Shape/full_rectangle.h>
#include <Shape/full_text.h>

class Game_Screen : public Screen {
protected:
	virtual void change_theme();

private:
	static bool grid;
	static bool range;
	static bool scan;
	static unsigned int opacity;
	static Full_Rectangle grids[50];
	static Full_Rectangle ranges[4];

	static Full_Rectangle scan_rect;
	static Full_Text scan_txt;

	static candle::LightingArea fog;
	static candle::RadialLight light;

	static sf::Clock clock;
	const static float KEY_PRESS_TIME_LIMIT;

	/*
	* Change opacity of the UI.
	*/
	void change_opacity();

	/*
	* Change range display due to the player's weapon.
	*/
	static void change_range();

public:
	Game_Screen();

	bool click_event_handler();

	void hover_event_handler();

	void draw();

	void key_event_handler();

	void update_draw();
};

#endif