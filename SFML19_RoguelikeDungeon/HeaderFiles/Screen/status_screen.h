#include "screen.h"
#include <SFML/Graphics/View.hpp>
#include <Shape/full_text.h>

#ifndef STATUS_SCREEN_H
#define STATUS_SCREEN_H

/**
* Represents a screen to view player's stats.
*/
class Status_Screen : public Screen {
private:
	static sf::View viewEffect;
	static Full_Text effect_txt;

protected:
	void changeTheme();

public:
	Status_Screen();

	bool handleClickEvent();

	void updateDraw();

	void handleMouseEvent();

	void draw();
};

#endif