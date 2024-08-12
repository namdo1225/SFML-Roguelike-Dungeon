/**
*
* File: exit_screen.cpp
* Description: Contain the implementation of the Exit_Screen class.
*/

#include "Screen/exit_screen.h"
#include <Screen/screen.h>
#include <SFML/Graphics/Color.hpp>

Exit_Screen::Exit_Screen() : Screen(false, false) {
	setup_helper(NULL, 100.f, 100.f, 1000.f, 600.f, false, true);
	rects[0].setFillColor(sf::Color(0, 0, 0, 128));
	rects[0].setOutlineThickness(0.f);

	setup_helper(NULL, 200.f, 200.f, 800.f, 400.f);
	setup_helper("Unsaved changes will be lost.\nDo you still want to quit?", 450, 220, NULL, NULL);

	setupTextbox("Back", 470.f, 340.f, 280.f, 40.f, []() {
		return_to_prev_screen(ExitScreen);
	});
	setupTextbox("Title", 470.f, 420.f, 280.f, 40.f, []() {
		switch_screen(ExitScreen, TitleScreen, false, true);
		for (unsigned int i = NameScreen; i < num_screens; i++)
			visibilities[i] = false;
	});
	setupTextbox("Quit", 470.f, 500.f, 280.f, 40.f, []() {
		window.close();
	});

	rects[0].setThemeAndHover(false);
	rects[1].setThemeAndHover(false);
	texts[0].setThemeAndHover(false);
}

void Exit_Screen::click_event_handler() {}

void Exit_Screen::hover_event_handler() {}