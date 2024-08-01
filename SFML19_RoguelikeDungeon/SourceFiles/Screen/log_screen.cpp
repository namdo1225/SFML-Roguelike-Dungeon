/**
*
* File: log_screen.cpp
* Description: Contain the implementation of the Log_Screen class.
*/

#include "Screen/log_screen.h"

const unsigned int Log_Screen::MAX_LOGS;

Log_Screen::Log_Screen() : Screen(2, 4, true, false) {
	setup_helper(0, NULL, 570.f, 5.f, 630.f, 200.f, false, true);
	rects[0].setFillColor(sf::Color(0, 0, 0, 128));
	rects[0].setOutlineThickness(0.f);

	setup_helper(1, NULL,  50.f, -10.f,  500.f,  810.f);
	setup_helper(2, NULL,  590.f,  10.f, 500.f,   50.f);
	setup_helper(3, NULL,  590.f,  90.f, 180.f,   50.f);

	setup_helper(0, "Only the last 50 logs are stored", 600.f, 20.f, 30.f, NULL);
	setup_helper(1, "Clear Log", 600.f, 100.f, 30.f, NULL);

	rects[0].setThemeAndHover(false);
	rects[1].setThemeAndHover(false);
	texts[0].setThemeAndHover(false);
}

void Log_Screen::click_event_handler() {
	if (mouse_in_button(ExitButton)) {
		viewLog.reset(sf::FloatRect(0, 0, 1200, 800));
		log_view(false);
		return_to_prev_screen(LogScreen);
	}
	else if (mouse_in_helper(true, 3))
		logs.clear();
}

void Log_Screen::hover_event_handler() {
	hover_button(ExitButton);
	hover_button(ConfirmButton);
	hover_textbox(3, 1);
}

void Log_Screen::draw() {
	Screen::draw();

	window.setView(viewLog);
	for (Full_Text& log : logs)
		window.draw(log);
	window.setView(viewUI);
}

void Log_Screen::mouse_event_handler() {
	if (event.type == sf::Event::MouseWheelScrolled) {
		const float delta = event.mouseWheelScroll.delta;
		viewLog.move(sf::Vector2f(0.f, delta * 5));
	}
}