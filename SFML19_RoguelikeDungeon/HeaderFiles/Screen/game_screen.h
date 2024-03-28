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
	bool grid = false;
	bool range = false;
	bool scan = false;
	unsigned int opacity = 255;
	Full_Rectangle grids[50];
	Full_Rectangle ranges[4];

	Full_Rectangle scan_rect = Full_Rectangle(10.f, 530.f, 100.f, 160.f);
	Full_Text scan_txt = Full_Text(20.f, 540.f, 18.f, "");

	/*
	* Change opacity of the UI.
	*/
	void change_opacity();

	/*
	* Change range display due to the player's weapon.
	*/
	void change_range();

public:
	Game_Screen();

	virtual void click_event_handler();

	virtual void hover_event_handler();

	virtual void draw();

	virtual void key_event_handler();

	virtual void update_draw();
};

#endif