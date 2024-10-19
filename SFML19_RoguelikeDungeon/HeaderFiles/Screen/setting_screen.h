#include "screen.h"
#include <string>

#ifndef SETTING_SCREEN_H
#define SETTING_SCREEN_H

/**
* Represents the setting screen.
*/
class Setting_Screen : public Screen {
private:

public:
	Setting_Screen();

	bool handleClickEvent();

	void updateDraw();

	static bool light_mode;
	static unsigned int theme;
	static unsigned int sfxVolume;
	static unsigned int musicVolume;
	static unsigned int font;
	static std::string saveLocation;
};

#endif