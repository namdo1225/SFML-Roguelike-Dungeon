/**
*
* File: exit_screen.cpp
* Description: Contain the implementation of the Exit_Screen class.
*/

#include "Screen/exit_screen.h"
#include <Screen/screen.h>
#include <SFML/Graphics/Color.hpp>

Exit_Screen::Exit_Screen() : Screen(false, false) {
	textRectH(NULL, 0.f, 0.f, 1200.f, 800.f, false, true);
	rects[0].setFillColor(sf::Color(0, 0, 0, 128));
	rects[0].setOutlineThickness(0.f);

	textRectH(NULL, 200.f, 200.f, 800.f, 400.f);
	textRectH("Unsaved changes will be lost.\nDo you still want to quit?", 450, 220, NULL, NULL);

	textboxH("Back", 470.f, 340.f, 280.f, 40.f, []() {
		goToPrevScreen(ExitScreen);
	});
	textboxH("Title", 470.f, 420.f, 280.f, 40.f, []() {
		switchScreen(ExitScreen, TitleScreen, false, true);
		for (unsigned int i = NameScreen; i < num_screens; i++)
			visibilities[i] = false;
	});
	textboxH("Quit", 470.f, 500.f, 280.f, 40.f, []() {
		window.close();
	});

	rects[0].setThemeAndHover(false);
	rects[1].setThemeAndHover(false);
	texts[0].setThemeAndHover(false);
}