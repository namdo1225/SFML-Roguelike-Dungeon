/**
*
* File: custom_screen.cpp
* Description: Contain the implementation of the Custom_Screen class.
*/

#include "Screen/custom_screen.h"
#include "interface.h"

Custom_Screen::Custom_Screen() : Screen(false, false) {
	setup_helper(NULL, 100.f, 100.f, 1000.f, 600.f, false, true);
	rects[0].setFillColor(sf::Color(0, 0, 0, 128));
	rects[0].setOutlineThickness(0.f);

	setup_helper(NULL, 200.f, 200.f, 800.f, 400.f);
	setup_helper(NULL, 500.f, 340.f, 200.f, 40.f);
	setup_helper(NULL, 500.f, 420.f, 200.f, 40.f);
	setup_helper(NULL, 500.f, 500.f, 200.f, 40.f);

	setup_helper("Unsaved changes will be lost.\nDo you still want to quit?", 450, 220, NULL, NULL);
	setup_helper("Back", 575.f, 343.f, NULL, NULL);
	setup_helper("Title", 573.f, 423.f, NULL, NULL);
	setup_helper("Quit", 575.f, 503.f, NULL, NULL);

	rects[1].setThemeAndHover(false);
	texts[0].setThemeAndHover(false);
}

void Custom_Screen::click_event_handler() {
	if (mouse_in_helper(true, 2))
		return_to_prev_screen(ExitScreen);
	else if (mouse_in_helper(true, 3)) {
		switch_screen(ExitScreen, TitleScreen, false, true);
		for (unsigned int i = NameScreen; i < num_screens; i++)
			visibilities[i] = false;
	}
	else if (mouse_in_helper(true, 4))
		window.close();
}

void Custom_Screen::hover_event_handler() {
	for (unsigned int i = 2; i < 5; i++)
		hover_textbox(i, i - 1);
}