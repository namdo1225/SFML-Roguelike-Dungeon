#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include "Candle/LightingArea.hpp"
#include "Candle/RadialLight.hpp"
#include "screen.h"
#include <SFML/System/Clock.hpp>
#include <Shape/full_rectangle.h>
#include <Shape/full_text.h>

/**
* Represents the main gameplay screen.
*/
class Game_Screen : public Screen {
protected:
	void changeTheme();

private:
	static bool grid;
	static bool range;
	static bool scan;
	static bool enemyFound;

	static unsigned int opacity;
	static Full_Rectangle grids[60];
	static Full_Rectangle rangeBox;

	static Full_Rectangle rectScan;
	static Full_Text scanText;

	static candle::LightingArea fog;
	static candle::RadialLight light;

	static sf::Clock clock;
	const static float KEY_PRESS_TIME_LIMIT;

	static Full_Text atkIcon;

	/*
	* Change opacity of the UI.
	*/
	void changeOpacity();

	/*
	* Change range display due to the player's weapon.
	*/
	static void changeRange();

	/*
	* Change grid display due to the player's zoom.
	*/
	static void changeGrid();

public:
	Game_Screen();

	bool handleClickEvent();

	void handleHoverEvent();

	void draw();

	void handleKeyEvent();

	void updateDraw();
};

#endif