/**
*
* File: name_screen.cpp
* Description: Contain the implementation of the Name_Screen class.
*/


#include "Manager/game_manager.h"
#include "Screen/name_screen.h"
#include <Screen/screen.h>
#include <SFML/Window/Keyboard.hpp>
#include <Shape/full_textinput.h>

Name_Screen::Name_Screen() : Screen(true, true, true, true) {
	setup_helper(
		"Enter your name (<= 20 chars, ASCII). Press 'Enter' or click 'Confirm' when finished."
		"\n\nAlphanumeric Only.",
		20.f, 20.f, false, NULL);
	setupTextInput("Player", 20, 300.f, 300.f, 500.f, 50.f, AlphanumericValidation);
}

bool Name_Screen::click_event_handler() {
	if (mouse_in_button(ExitButton)) {
		switch_screen(NameScreen, TitleScreen, false, true);
		textInputs[0].text.setString("Player");
		return true;
	}
	else if (mouse_in_button(ConfirmButton)) {
		switch_screen(NameScreen, StatScreen, false);
		Game_Manager::player.set_name(textInputs[0].text.getString().toAnsiString().c_str());
		textInputs[0].text.setString("Player");
		return true;
	}
	else if (mouse_in_button(ClearButton)) {
		textInputs[0].text.setString("");
		return true;
	}
	return false;
}

void Name_Screen::key_event_handler() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
		switch_screen(NameScreen, StatScreen, false, true);
		Game_Manager::player.set_name(textInputs[0].text.getString().toAnsiString().c_str());
		textInputs[0].text.setString("Player");
	}
}
