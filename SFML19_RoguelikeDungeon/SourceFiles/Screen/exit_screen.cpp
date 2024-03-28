/**
*
* File: exit_screen.cpp
* Description: Contain the implementation of the Exit_Screen class.
*/

#include "Screen/exit_screen.h"
#include "interface.h"

Exit_Screen::Exit_Screen() : Screen(4, 5, false, false) {
	setup_helper(true, 0, NULL, 100.f, 100.f, 1000.f, 600.f, 0, false, false, true);
	rects[0].setFillColor(sf::Color(0, 0, 0, 128));
	rects[0].setOutlineThickness(0.f);

	setup_helper(true, 1, NULL, 200.f, 200.f, 800.f, 400.f);
	setup_helper(true, 2, NULL, 500.f, 340.f, 200.f, 40.f);
	setup_helper(true, 3, NULL, 500.f, 420.f, 200.f, 40.f);
	setup_helper(true, 4, NULL, 500.f, 500.f, 200.f, 40.f);

	setup_helper(false, 0, "Unsaved changes will be lost.\nDo you still want to quit?", 450, 220, NULL, NULL);
	setup_helper(false, 1, "Back", 575.f, 343.f, NULL, NULL);
	setup_helper(false, 2, "Title", 573.f, 423.f, NULL, NULL);
	setup_helper(false, 3, "Quit", 575.f, 503.f, NULL, NULL);

	rects[1].setThemeAndHover(Setting_Manager::theme, false);
	texts[0].setThemeAndHover(Setting_Manager::theme, false);
}

void Exit_Screen::click_event_handler() {
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

void Exit_Screen::hover_event_handler() {
	for (unsigned int i = 2; i < 5; i++)
		hover_textbox(i, i - 1);
}