/**
*
* File: spell_screen.cpp
* Description: Contain the implementation of the Spell_Screen class.
*/


#include "Manager/game_manager.h"
#include "Screen/spell_screen.h"
#include <format>
#include <Screen/screen.h>
#include <Shape/full_rectangle.h>
#include <string>
#include <Tool/spell.h>

Spell_Screen::Spell_Screen() : Screen(true, false) {
	update = true;
	textRectH("Spells", 360.f, 20.f, 36.f, NULL);
	textRectH("0 / 32 spells", 340.f, 150.f, 24.f, NULL);
    textRectH("", 50.f, 720.f, 24.f, NULL);
}

bool Spell_Screen::handleClickEvent() {
    if (mouseInButton(ExitButton)) {
        Game_Manager::selectedSpell = NULL;
        texts[2].setString("");
        switchScreen(SpellScreen, GameScreen, false, true);
        return true;
    }
    else if (mouseInButton(UseButton) && Game_Manager::selectedSpell) {
        if (Game_Manager::selectedSpell->getType() == Functional) {
            std::string spell_name = Game_Manager::selectedSpell->getName();
            texts[2].setString(Game_Manager::useSpell() ? std::format("You used {}.", spell_name) :
                    std::format("You failed to cast {}.", spell_name));
        }
        else {
            switchScreen(SpellScreen, GameScreen, false, true);
            openDialog(GameScreen, SpellAttackScreen);
        }
        return true;
    }
    else if (mouseInButton(DiscardButton) && Game_Manager::selectedSpell) {
        Game_Manager::delSelectedSpell();
        return true;
    }

    for (Spell& spell : Game_Manager::spells) {
        if (spell.contains(x, y)) {
            // Swaps spell position
            if (Game_Manager::selectedSpell && &spell != Game_Manager::selectedSpell) {
                int sx = Game_Manager::selectedSpell->getPos('x'), sy = Game_Manager::selectedSpell->getPos('y');
                Game_Manager::selectedSpell->setPos(spell.getPos('x'), spell.getPos('y'));
                spell.setPos(sx, sy);
                Game_Manager::selectedSpell = NULL;
                return true;
            }
            // Selects a spell
            else if (!Game_Manager::selectedSpell) {
                int sx = spell.getPos('x'), sy = spell.getPos('y');
                map_rects["inv_sp_cur_slot"].setPosition(sx - 5, sy - 5);
                map_txts["inv_sp_detail"].setString(spell.getDesc());
                Game_Manager::selectedSpell = &spell;
                return true;
            }
            // Unselects
            else if (&spell == Game_Manager::selectedSpell) {
                Game_Manager::selectedSpell = NULL;
                return true;
            }
        }
    }
}

void Spell_Screen::handleHoverEvent() {
	hoverButton(UseButton);
	hoverButton(DiscardButton);

	for (unsigned int i = 0; i < MAX_INV_SPELL_SLOTS; i++)
		hoverSlot(i);
}

void Spell_Screen::draw() {
    window.draw(map_rects["background"]);
    if (Game_Manager::selectedSpell)
        window.draw(map_rects["inv_sp_cur_slot"]);
	Screen::draw();

	for (Full_Rectangle& rect : inv_sp_slots)
		window.draw(rect);

    for (Spell& sp : Game_Manager::spells) {
        sp.draw();
    }      

    if (Game_Manager::selectedSpell) {
        window.draw(map_rects["inv_sp_desc"]);
        window.draw(map_txts["inv_sp_desc"]);
        window.draw(map_rects["inv_sp_use"]);
        window.draw(map_txts["inv_sp_use"]);
        window.draw(map_rects["inv_sp_discard"]);
        window.draw(map_txts["inv_sp_discard"]);
        window.draw(map_txts["inv_sp_detail"]);
    }

    window.draw(map_txts["inv_sp_gold"]);
    window.draw(map_txts["inv_sp_gold_amount"]);
}

void Spell_Screen::updateDraw() {
	texts[1].setString(std::format("{} / {} spells",
		Game_Manager::spells.size(), Game_Manager::MAX_INV_SPELL_SLOTS));

    map_txts["inv_sp_gold_amount"].setString(std::to_string(Game_Manager::player.getGold()));
}