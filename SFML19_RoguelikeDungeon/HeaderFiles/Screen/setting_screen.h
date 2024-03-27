/**
*
* File: setting_screen.h
* Description: Contain the declaration of the Setting_Screen class, which represents the exit dialog scene.
*/

#ifndef SETTING_SCREEN_H
#define SETTING_SCREEN_H

#include "screen.h"

class Setting_Screen : public Screen {
private:

public:
	Setting_Screen();

	void click_event_handler();

	void hover_event_handler();

	virtual void draw();

	bool light_mode = false;
	unsigned int theme = 0;
	unsigned int sfx_volume = 100;
	unsigned int music_volume = 100;
	unsigned int font = 0;
};

#endif