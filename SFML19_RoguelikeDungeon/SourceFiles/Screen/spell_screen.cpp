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
	setup_helper("Spells", 360.f, 20.f, 36.f, NULL);
	setup_helper("0 / 32 spells", 340.f, 150.f, 24.f, NULL);
    setup_helper("", 50.f, 720.f, 24.f, NULL);
}

bool Spell_Screen::click_event_handler() {
    if (mouse_in_button(ExitButton)) {
        Game_Manager::spell_select = NULL;
        texts[2].setString("");
        switch_screen(SpellScreen, GameScreen, false, true);
        return true;
    }
    else if (mouse_in_button(UseButton) && Game_Manager::spell_select) {
        if (Game_Manager::spell_select->type == Functional) {
            std::string spell_name = Game_Manager::spell_select->name;
            texts[2].setString(Game_Manager::spell_use() ? std::format("You used {}.", spell_name) :
                    std::format("You failed to cast {}.", spell_name));
        }
        else {
            switch_screen(SpellScreen, GameScreen, false, true);
            show_dialog(GameScreen, SpellAttackScreen);
        }
        return true;
    }
    else if (mouse_in_button(DiscardButton) && Game_Manager::spell_select) {
        Game_Manager::deleted_selected_sp();
        return true;
    }

    for (Spell& spell : Game_Manager::spells) {
        if (spell.contains(x, y)) {
            // Swaps spell position
            if (Game_Manager::spell_select && &spell != Game_Manager::spell_select) {
                int sx = Game_Manager::spell_select->getPos('x'), sy = Game_Manager::spell_select->getPos('y');
                Game_Manager::spell_select->setPos(spell.getPos('x'), spell.getPos('y'));
                spell.setPos(sx, sy);
                Game_Manager::spell_select = NULL;
                return true;
            }
            // Selects a spell
            else if (!Game_Manager::spell_select) {
                int sx = spell.getPos('x'), sy = spell.getPos('y');
                map_rects["inv_sp_cur_slot"].setPosition(sx - 5, sy - 5);
                map_txts["inv_sp_detail"].setString(spell.desc.c_str());
                Game_Manager::spell_select = &spell;
                return true;
            }
            // Unselects
            else if (&spell == Game_Manager::spell_select) {
                Game_Manager::spell_select = NULL;
                return true;
            }
        }
    }
}

void Spell_Screen::hover_event_handler() {
	hover_button(UseButton);
	hover_button(DiscardButton);

	for (unsigned int i = 0; i < MAX_INV_SPELL_SLOTS; i++)
		hover_slot(i);
}

void Spell_Screen::draw() {
    window.draw(map_rects["background"]);
    if (Game_Manager::spell_select)
        window.draw(map_rects["inv_sp_cur_slot"]);
	Screen::draw();

	for (Full_Rectangle& rect : inv_sp_slots)
		window.draw(rect);

    for (Spell& sp : Game_Manager::spells) {
        sp.draw();
    }      

    if (Game_Manager::spell_select) {
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

void Spell_Screen::update_draw() {
	texts[1].setString(std::format("{} / {} spells",
		Game_Manager::spells.size(), Game_Manager::MAX_INV_SPELL_SLOTS));

    map_txts["inv_sp_gold_amount"].setString(std::to_string(Game_Manager::player.get_gold()));
}