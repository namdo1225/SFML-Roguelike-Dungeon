/**
*
* File: name_screen.cpp
* Description: Contain the implementation of the Name_Screen class.
*/


#include "Screen/name_screen.h"
#include "Manager/game_manager.h"

Name_Screen::Name_Screen() : Screen(1, 1, true, true, true, true) {
	setup_helper(true, 0, NULL, 350.f, 240.f, 470.f, 50.f);

	setup_helper(false, 0,
		"Enter your name (<= 20 chars, ASCII). Press 'Enter' or click 'Confirm' when finished."
		"\n\nSome characters can't be typed:  \\  =  $  *  |  ~  .  \"  '  ;",
		20.f, 20.f, NULL, NULL);

	rects[0].setThemeAndHover(Setting_Manager::theme, false);
	texts[0].setThemeAndHover(Setting_Manager::theme, false);
}

void Name_Screen::click_event_handler() {
	if (mouse_in_button(ExitButton)) {
		switch_screen(NameScreen, TitleScreen, false, true);
		name_txt.setString("Player");
	}
	else if (mouse_in_button(ConfirmButton)) {
		switch_screen(NameScreen, StatScreen, false);
		Game_Manager::player.set_name(name_txt.getString().toAnsiString().c_str());
		name_txt.setString("Player");
	}
	else if (mouse_in_button(ClearButton)) {
		name_txt.setString("");
	}
}

void Name_Screen::hover_event_handler() {
	hover_button(ExitButton);
	hover_button(ConfirmButton);
	hover_button(ClearButton);
}

void Name_Screen::text_event_handler() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
		switch_screen(NameScreen, StatScreen, false, true);
		Game_Manager::player.set_name(name_txt.getString().toAnsiString().c_str());
		name_txt.setString("Player");
	}
	Screen::text_event_handler();
}
