/**
*
* File: log_screen.cpp
* Description: Contain the implementation of the Log_Screen class.
*/

#include "Screen/log_screen.h"
#include <Screen/screen.h>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <Shape/full_text.h>

Log_Screen::Log_Screen() : Screen(true, false) {
	textRectH(NULL, 570.f, 5.f, 630.f, 200.f, false, true);
	rects[0].setFillColor(sf::Color(0, 0, 0, 128));
	rects[0].setOutlineThickness(0.f);

	textRectH(NULL,  50.f, -10.f,  500.f,  810.f);
	textRectH(NULL,  590.f,  10.f, 500.f,   50.f);

	textRectH("Only the last 50 logs are stored.", 600.f, 20.f, 30.f, NULL);
	textboxH("Clear Log", 590.f, 90.f, 180.f, 50.f, []() {
		logs.clear();
	});
	textboxH("Reset Scroll", 790.f, 90.f, 180.f, 50.f, []() {
		viewLog.reset(sf::FloatRect(0, 0, DEFAULT_SCREEN_X, DEFAULT_SCREEN_Y));
	});
	textboxH("^", 590.f, 190.f, 50.f, 50.f, []() {
		viewLog.move(sf::Vector2f(0.f, 10.f));
	});
	textboxH("v", 590.f, 290.f, 50.f, 50.f, []() {
		viewLog.move(sf::Vector2f(0.f, -10.f));
	});

	rects[0].setThemeAndHover(false);
	rects[1].setThemeAndHover(false);
	texts[0].setThemeAndHover(false);
}

bool Log_Screen::handleClickEvent() {
	if (mouseInButton(ExitButton)) {
		viewLog.reset(sf::FloatRect(0, 0, 1200, 800));
		log_view(false);
		goToPrevScreen(LogScreen);
		return true;
	}
}

void Log_Screen::draw() {
	Screen::draw();

	window.setView(viewLog);
	for (Full_Text& log : logs)
		window.draw(log);
	window.setView(viewUI);
}

void Log_Screen::handleMouseEvent() {
	if (event.type == sf::Event::MouseWheelScrolled) {
		const float delta = event.mouseWheelScroll.delta;
		viewLog.move(sf::Vector2f(0.f, delta * 5));
	}
}