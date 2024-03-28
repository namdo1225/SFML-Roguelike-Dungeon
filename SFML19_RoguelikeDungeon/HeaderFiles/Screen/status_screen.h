/**
*
* File: status_screen.h
* Description: Contain the declaration of the Status_Screen class,
* which represents a screen to view player's stats.
*/

#ifndef STATUS_SCREEN_H
#define STATUS_SCREEN_H

#include "screen.h"

class Status_Screen : public Screen {
private:
	static sf::View viewEffect;
	Full_Text effect_txt = Full_Text(800.f, 150.f, 20.f, "");

protected:
	virtual void change_theme();

public:
	Status_Screen();

	virtual void click_event_handler();

	virtual void hover_event_handler();

	virtual void update_draw();

	virtual void mouse_event_handler();

	virtual void draw();
};

#endif