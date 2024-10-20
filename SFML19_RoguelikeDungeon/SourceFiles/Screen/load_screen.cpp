/**
*
* File: load_screen.cpp
* Description: Contain the implementation of the Load_Screen class.
*/

#include "Screen/load_screen.h"
#include "Manager/game_manager.h"

Load_Screen::Load_Screen() : Screen(1, 1, true, true, true, true) {
	setup_helper(true, 0, NULL, 350.f, 240.f, 470.f, 50.f);

	setup_helper(false, 0,
		"Enter your save's name. Make sure the save file (.sav) is in"
		"\nthe same folder as the EXE file."
		"\n\nSome characters can't be typed:  \\  =  $  *  |  ~  .  \"  '  ;",
		20.f, 20.f, NULL, NULL);

	rects[0].setThemeAndHover(Setting_Manager::theme, false);
	texts[0].setThemeAndHover(Setting_Manager::theme, false);
}

void Load_Screen::click_event_handler() {
	if (mouse_in_button(ExitButton)) {
		return_to_prev_screen(LoadScreen);
		name_txt.setString("Player");
	}
	else if (mouse_in_button(ConfirmButton)) {
		switch_screen(LoadScreen,
			Game_Manager::read_save(name_txt.getString().toAnsiString().c_str())
			? GameScreen
			: TitleScreen,
			false, true);
		name_txt.setString("Player");
	}
	else if (mouse_in_button(ClearButton)) {
		name_txt.setString("");
	}
}

void Load_Screen::hover_event_handler() {
	hover_button(ExitButton);
	hover_button(ConfirmButton);
	hover_button(ClearButton);
}

void Load_Screen::text_event_handler() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
		if (Game_Manager::read_save(name_txt.getString().toAnsiString().c_str()))
			switch_screen(LoadScreen, GameScreen, false, true);
		name_txt.setString("Player");
	}
	Screen::text_event_handler();
}