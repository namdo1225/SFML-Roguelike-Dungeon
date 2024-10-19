/**
*
* File: spell_attack_screen.cpp
* Description: Contain the implementation of the Spell_Attack_Screen class.
*/

#include "Manager/game_manager.h"
#include "Screen/spell_attack_screen.h"
#include <format>
#include <Screen/screen.h>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <Shape/full_rectangle.h>
#include <stat.h>
#include <string>
#include <Tool/spell.h>

Full_Rectangle Spell_Attack_Screen::rangeBox;
int Spell_Attack_Screen::spellID = -1;

Spell_Attack_Screen::Spell_Attack_Screen() : Screen(true, false) {
	textRectH("Spell range: ", 200.f, 10.f, NULL, 3.f);
	textRectH("0", 400, 10.f, NULL, 3.f);
	textRectH(" ", 200, 50.f, 12.f, 3.f);

	rangeBox = Full_Rectangle(600.f, 400.f, 40.f, 40.f, false, true, sf::Color::Transparent, sf::Color(255, 0, 0, 100));
}

bool Spell_Attack_Screen::handleClickEvent() {
	Spell* selected = Game_Manager::selectedSpell != SelectNone ? &Game_Manager::spells[Game_Manager::selectedSpell] : NULL;

	if (mouseInButton(ExitButton)) {
		resetSpell();
		return true;
	}
	else if (selected && selected->getMP() <= Game_Manager::player.getStat(Mp)) {
		unsigned int range = selected->getRange() * 40;
		unsigned int plX = Game_Manager::player.getPos('x');
		unsigned int plY = Game_Manager::player.getPos('y');

		if (rangeBox.getGlobalBounds().contains(worldX, worldY)) {
			for (unsigned int i{ 0 }; i < Game_Manager::enemies.size(); i++)
				if (Game_Manager::enemies[i].contains(worldX, worldY)) {
					Game_Manager::atkWithSpell(i);
					resetSpell();
					Game_Manager::handleTurn();
					return true;
				}
			log_add("No target in range.");
			return true;
		}

		log_add("Out of range.");
		return true;
	}
	else if (selected && selected->getMP() > Game_Manager::player.getStat(Mp)) {
		log_add(std::format("Insufficient MP for spell: requires {}.", selected->getMP()).c_str());
		resetSpell();
		return true;
	}
}

void Spell_Attack_Screen::handleHoverEvent() {
	Spell* selected = Game_Manager::selectedSpell != SelectNone ? &Game_Manager::spells[Game_Manager::selectedSpell] : NULL;
	if (selected && spellID != selected->getID())
		changeRange();
}

void Spell_Attack_Screen::draw() {
	Screen::draw();

	window.setView(viewWorld);
	if (spellID > SelectNone)
		window.draw(rangeBox);
	window.setView(viewUI);
}

void Spell_Attack_Screen::changeRange() {
	Spell* selected = Game_Manager::selectedSpell != SelectNone ? &Game_Manager::spells[Game_Manager::selectedSpell] : NULL;
	if (!selected)
		return;

	window.setView(viewWorld);

	spellID = selected->getID();
	unsigned int originalRange = selected->getRange();
	float rangeArea = (originalRange * 2 + 1) * TILE;

	unsigned int plX = Game_Manager::player.getPos('x');
	unsigned int plY = Game_Manager::player.getPos('y');

	rangeBox.setPosition(plX - originalRange * TILE, plY - originalRange * TILE);
	rangeBox.setSize(sf::Vector2f(rangeArea, rangeArea));

	texts[1].setString(std::to_string(originalRange));
	texts[2].setString(selected->getDesc());

	window.setView(viewUI);
}

void Spell_Attack_Screen::resetSpell() {
	Game_Manager::selectedSpell = spellID = SelectNone;
	goToPrevScreen(SpellAttackScreen);
}
