/**
*
* File: load_screen.cpp
* Description: Contain the implementation of the Load_Screen class.
*/

#include "Manager/game_manager.h"
#include "Screen/load_screen.h"
#include <Screen/screen.h>
#include <SFML/Window/Keyboard.hpp>

Load_Screen::Load_Screen() : Screen(true, true, true) {
	setup_helper(
		"When you click 'Confirm', you will be asked to select a .sav file."
		"\nYou will be directed to play the game if the save loads successfully.",
		150.f, 300.f, NULL, NULL);

	texts[0].setThemeAndHover(false);
}

bool Load_Screen::click_event_handler() {
	if (mouse_in_button(ExitButton)) {
		return_to_prev_screen(LoadScreen);
		return true;
	}
	else if (mouse_in_button(ConfirmButton)) {
		Game_Manager::read_save() ? switch_screen(LoadScreen, GameScreen, false, true) :
			showMessage(LoadScreen, "You cancelled the save dialog.", ErrorMsg);
		return true;
	}
	return false;
}

void Load_Screen::text_event_handler() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && Game_Manager::read_save())
		switch_screen(LoadScreen, GameScreen, false, true);
}