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
#include <string>

Tool::Tool(std::string name, std::string desc, std::string abbrev,
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
	icon.changeTheme();
}

std::string Tool::getName() {
	return name;
}

std::string Tool::getOriginalDesc() {
	return originalDesc;
}

std::string Tool::getDesc() {
	return desc;
}

std::string Tool::getAbbrev() {
	return abbrev;
}

unsigned int Tool::getID() {
	return id;
}

unsigned int Tool::getRange() {
	return range;
}

unsigned int Tool::getBuy() {
	return buy;
}

unsigned int Tool::getSell() {
	return sell;
}

int Tool::getQuantity() {
	return quantity;
}

unsigned int Tool::getType() {
	return 0;
}
