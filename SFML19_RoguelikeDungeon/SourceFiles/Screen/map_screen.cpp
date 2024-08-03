/**
*
* File: map_screen.cpp
* Description: Contain the implementation of the Map_Screen class.
*/

#include "Screen/map_screen.h"
#include "Manager/game_manager.h"
#include <iostream>

sf::Vector2f Map_Screen::oldPos;
bool Map_Screen::moving = false;
sf::View Map_Screen::viewMap;

Map_Screen::Map_Screen() : Screen(true, false) {
	setupTextbox("^", 1040.f, 490.f, 50.f, 50.f, []() {
		viewMap.move(sf::Vector2f(0.f, 10.f));
	});
	setupTextbox(">", 1110.f, 565.f, 50.f, 50.f, []() {
		viewMap.move(sf::Vector2f(-10.f, 0.f));
	});
	setupTextbox("V", 1040.f, 640.f, 50.f, 50.f, []() {
		viewMap.move(sf::Vector2f(0.f, -10.f));
	});
	setupTextbox("<", 965.f, 565.f, 50.f, 50.f, []() {
		viewMap.move(sf::Vector2f(10.f, 0.f));
	});
	setupTextbox("+", 965.f, 415.f, 50.f, 50.f, []() {
		viewMap.zoom(0.5f);
	});
	setupTextbox("-", 1110.f, 415.f, 50.f, 50.f, []() {
		viewMap.zoom(2.f);
	});
}

void Map_Screen::click_event_handler() {
	if (mouse_in_button(ExitButton)) {
		viewMap.reset(sf::FloatRect(0, 0, 1200, 800));
		switch_screen(MapScreen, GameScreen, false, true);
	}
}

void Map_Screen::mouse_event_handler() {
	switch (event.type) {
	case sf::Event::MouseButtonPressed:
		if (event.mouseButton.button == 0) {
			moving = true;
			oldPos = sf::Vector2f(sf::Mouse::getPosition(window));
		}
		break;
	case sf::Event::MouseButtonReleased:
		if (event.mouseButton.button == 0)
			moving = false;
		break;
	case sf::Event::MouseMoved:
	{
		if (!moving)
			break;

		const sf::Vector2f newPos = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
		sf::Vector2f deltaPos = oldPos - newPos;

		viewMap.move(deltaPos);
		oldPos = newPos;
		break;
	}
	}
}

void Map_Screen::draw() {
	window.draw(map_rects["background"]);
	window.setView(viewMap);
	Game_Manager::floor.map.draw();
	window.setView(viewUI);
	Screen::draw();
}