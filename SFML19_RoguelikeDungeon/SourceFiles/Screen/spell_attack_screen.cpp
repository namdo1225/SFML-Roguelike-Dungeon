/**
*
* File: spell_attack_screen.cpp
* Description: Contain the implementation of the Spell_Attack_Screen class.
*/

#include "Manager/game_manager.h"
#include "Screen/spell_attack_screen.h"
#include <array>
#include <format>
#include <Screen/screen.h>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <Shape/full_rectangle.h>
#include <stat.h>
#include <string>

std::array<int, 3> Spell_Attack_Screen::atkSpInfo = {{0, 0, 0}};
Full_Rectangle Spell_Attack_Screen::ranges[4];

Spell_Attack_Screen::Spell_Attack_Screen() : Screen(true, false) {
	textRectH("Spell range: ", 200.f, 10.f, NULL, 3.f);
	textRectH("0", 400, 10.f, NULL, 3.f);

	unsigned int range = atkSpInfo[1];
	ranges[0] = Full_Rectangle(400.f, 400.f - (40.f * range), 40.f, 40.f * range, false, true, sf::Color::Transparent, sf::Color(255, 0, 0, 100));
	ranges[1] = Full_Rectangle(440.f, 400.f, 40.f * range, 40.f, false, true, sf::Color::Transparent, sf::Color(255, 0, 0, 100));
	ranges[2] = Full_Rectangle(400.f, 440.f, 40.f, 40.f * range, false, true, sf::Color::Transparent, sf::Color(255, 0, 0, 100));
	ranges[3] = Full_Rectangle(400.f - (40.f * range), 400.f, 40.f * range, 40.f, false, true, sf::Color::Transparent, sf::Color(255, 0, 0, 100));
}

bool Spell_Attack_Screen::handleClickEvent() {
	if (mouseInButton(ExitButton)) {
		resetSpell();
		return true;
	}
	else if (atkSpInfo[2] > Game_Manager::player.getStat(Mp)) {
		log_add(std::format("Insufficient MP for spell: requires {}.", atkSpInfo[3]).c_str());
		resetSpell();
		return true;
	}
	else if (x >= 400 - (atkSpInfo[1] * 40) && x <= 440 + (atkSpInfo[1] * 40) && y >= 400 - (atkSpInfo[1] * 40) && y <= 440 + (atkSpInfo[1] * 40))
		for (unsigned int i{ 0 }; i < Game_Manager::enemies.size(); i++)
			if (Game_Manager::enemies[i].contains(worldX, worldY)) {
				Game_Manager::atkWithSpell(i, atkSpInfo);
				resetSpell();
				Game_Manager::handleTurn();
				return true;
			}
}

void Spell_Attack_Screen::handleHoverEvent() {
	if (Game_Manager::selectedSpell != NULL && atkSpInfo[1] == 0) {
		atkSpInfo = Game_Manager::selectedSpell->atk();
		changeRange();
	}
}

void Spell_Attack_Screen::draw() {
	Screen::draw();

	if (atkSpInfo[1] != 0)
		for (Full_Rectangle& rect : ranges)
			window.draw(rect);
}

void Spell_Attack_Screen::changeRange() {
	unsigned int range = atkSpInfo[1];
	ranges[0].setPosition(400.f, 400.f - (40.f * range));
	ranges[0].setSize(sf::Vector2f(40.f, 40.f * range));

	ranges[1].setPosition(440.f, 400.f);
	ranges[1].setSize(sf::Vector2f(40.f * range, 40.f));

	ranges[2].setPosition(400.f, 440.f);
	ranges[2].setSize(sf::Vector2f(40.f, 40.f * range));

	ranges[3].setPosition(400.f - (40.f * range), 400.f);
	ranges[3].setSize(sf::Vector2f(40.f * range, 40.f));

	texts[1].setString(std::to_string(range));
}

void Spell_Attack_Screen::resetSpell() {
	Game_Manager::selectedSpell = NULL;
	atkSpInfo = { 0, 0, 0 };
	goToPrevScreen(SpellAttackScreen);
}
