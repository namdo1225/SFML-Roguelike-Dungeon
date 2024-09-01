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

	bool click_event_handler();

	void update_draw();

	static bool light_mode;
	static unsigned int theme;
	static unsigned int sfx_volume;
	static unsigned int music_volume;
	static unsigned int font;
};

#endif