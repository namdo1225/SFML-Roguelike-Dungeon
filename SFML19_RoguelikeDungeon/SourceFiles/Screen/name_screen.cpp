/**
*
* File: name_screen.cpp
* Description: Contain the implementation of the Name_Screen class.
* 
*/


#include "Manager/game_manager.h"
#include "Screen/name_screen.h"
#include <Screen/screen.h>
#include <SFML/Window/Keyboard.hpp>
#include <Shape/full_textinput.h>

Name_Screen::Name_Screen() : Screen(true, true, true, true) {
	textRectH(
		"Enter your name (<= 20 chars, ASCII). Press 'Enter' or click 'Confirm' when finished."
		"\n\nAlphanumeric Only.",
		20.f, 20.f, false, NULL);
	textInputH("Player", 20, 300.f, 300.f, 500.f, 50.f, AlphanumericValidation);
}

bool Name_Screen::handleClickEvent() {
	if (mouseInButton(ExitButton)) {
		switchScreen(NameScreen, TitleScreen, false, true);
		textInputs[0].text.setString("Player");
		return true;
	}
	else if (mouseInButton(ConfirmButton)) {
		switchScreen(NameScreen, StatScreen, false);
		Game_Manager::player.setName(textInputs[0].text.getString().toAnsiString().c_str());
		textInputs[0].text.setString("Player");
		return true;
	}
	else if (mouseInButton(ClearButton)) {
		textInputs[0].text.setString("");
		return true;
	}
	return false;
}

void Name_Screen::handleKeyEvent() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
		switchScreen(NameScreen, StatScreen, false, true);
		Game_Manager::player.setName(textInputs[0].text.getString().toAnsiString().c_str());
		textInputs[0].text.setString("Player");
	}
}
