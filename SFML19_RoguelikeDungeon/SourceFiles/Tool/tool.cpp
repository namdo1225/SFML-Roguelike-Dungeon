/**
*
* File: tool.cpp
* Description: Contains the implementation of the Tool class.
*
*/

#include "Manager/font_manager.h"
#include "Manager/game_manager.h"
#include "Tool/tool.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Text.hpp>

Tool::Tool(const char* name, const char* desc, const char* abbrev,
	unsigned int id, unsigned int buy, unsigned int sell, int quantity,
	unsigned int range) : id(id),
	name(name), desc(desc), abbrev(abbrev), range(range), buy(buy), sell(sell),
	quantity(quantity) {
	icon.setFillColor(sf::Color::White);
	icon.setStyle(sf::Text::Bold);
	icon.setCharacterSize(30);

	icon.setFont(Font_Manager::get_selected());
	icon.setString(abbrev);

	changeTheme();
}

void Tool::draw() const {
	Game_Manager::window.draw(boundRect);
	Game_Manager::window.draw(icon);
}

int Tool::getPos(char z) {
	return z == 'x' ? boundRect.getPosition().x : boundRect.getPosition().y;;
}

void Tool::setPos(float x, float y) {
	icon.setPosition(x + 5, y + 5);
	boundRect.setPosition(x, y);
}

bool Tool::contains(float x, float y) {
	return boundRect.getGlobalBounds().contains(x, y);
}

void Tool::changeTheme() {
	icon.flip_theme();
}
