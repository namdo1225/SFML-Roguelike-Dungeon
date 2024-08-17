#include "Screen/message_screen.h"
#include <Screen/screen.h>
#include <SFML/Graphics/Color.hpp>

Message_Screen::Message_Screen() : Screen(false, false) {
	setup_helper(NULL, 0.f, 0.f, 1200.f, 800.f, false);
	rects[0].setFillColor(sf::Color(0, 0, 0, 128));
	rects[0].setOutlineThickness(0.f);

	setup_helper(NULL, 200.f, 200.f, 800.f, 400.f, false, true);
	setup_helper("", 420.f, 300.f, NULL, NULL, false, true);

	setupTextbox("Return", 420.f, 540.f, 380.f, 40.f, []() {
		return_to_prev_screen(MessageScreen);
	});
}