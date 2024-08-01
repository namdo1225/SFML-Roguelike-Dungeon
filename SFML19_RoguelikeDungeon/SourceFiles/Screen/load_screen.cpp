/**
*
* File: load_screen.cpp
* Description: Contain the implementation of the Load_Screen class.
*/

#include "Screen/load_screen.h"
#include "Manager/game_manager.h"

Load_Screen::Load_Screen() : Screen(true, true, true, true) {
	setup_helper(NULL, 350.f, 240.f, 470.f, 50.f);

	setup_helper(
		"Enter your save's name. Make sure the save file (.sav) is in"
		"\nthe same folder as the EXE file."
		"\n\nSome characters can't be typed:  \\  =  $  *  |  ~  .  \"  '  ;",
		20.f, 20.f, NULL, NULL);

	rects[0].setThemeAndHover(false);
	texts[0].setThemeAndHover(false);
}

void Load_Screen::click_event_handler() {
	if (mouse_in_button(ExitButton)) {
		return_to_prev_screen(LoadScreen);
		map_txts["name"].setString("Player");
	}
	else if (mouse_in_button(ConfirmButton)) {
		switch_screen(LoadScreen,
			Game_Manager::read_save(map_txts["name"].getString().toAnsiString().c_str())
			? GameScreen
			: TitleScreen,
			false, true);
		map_txts["name"].setString("Player");
	}
	else if (mouse_in_button(ClearButton)) {
		map_txts["name"].setString("");
	}
}

void Load_Screen::hover_event_handler() {
	hover_button(ExitButton);
	hover_button(ConfirmButton);
	hover_button(ClearButton);
}

void Load_Screen::text_event_handler() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
		if (Game_Manager::read_save(map_txts["name"].getString().toAnsiString().c_str()))
			switch_screen(LoadScreen, GameScreen, false, true);
		map_txts["name"].setString("Player");
	}
	Screen::text_event_handler();
}