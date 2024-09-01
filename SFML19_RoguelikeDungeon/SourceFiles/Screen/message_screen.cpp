/**
*
* File: message_screen.cpp
* Description: Contains the implementation of the Message_Screen class.
*
*/

#include "Screen/message_screen.h"
#include <Screen/screen.h>
#include <SFML/Graphics/Color.hpp>

Message_Screen::Message_Screen() : Screen(false, false) {
	textRectH(NULL, 0.f, 0.f, 1200.f, 800.f, false);
	rects[0].setFillColor(sf::Color(0, 0, 0, 128));
	rects[0].setOutlineThickness(0.f);

	textRectH(NULL, 200.f, 200.f, 800.f, 400.f, false, true);
	textRectH("", 420.f, 300.f, NULL, NULL, false, true);

	textboxH("Return", 420.f, 540.f, 380.f, 40.f, []() {
		goToPrevScreen(MessageScreen);
	});
}