/**
*
* File: game_screen.h
* Description: Contain the declaration of the Game_Screen class, which represents the main gameplay screen.
*/

#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include "screen.h"

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

	virtual void click_event_handler();

	virtual void hover_event_handler();

	virtual void draw();

	virtual void key_event_handler();

	virtual void update_draw();
};

#endif