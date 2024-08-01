/**
*
* File: spell_screen.cpp
* Description: Contain the implementation of the Spell_Screen class.
*/


#include "Screen/spell_screen.h"
#include <format>
#include "Manager/game_manager.h"

Spell_Screen::Spell_Screen() : Screen(3, 0, true, false) {
	update = true;
	setup_helper(0, "Spells", 360.f, 20.f, 36.f, NULL);
	setup_helper(1, "0 / 32 spells", 340.f, 150.f, 24.f, NULL);
    setup_helper(2, "", 50.f, 720.f, 24.f, NULL);
}

void Spell_Screen::click_event_handler() {
    if (mouse_in_button(ExitButton)) {
        Game_Manager::spell_select = Game_Manager::spell_desc = Game_Manager::placeholder_spell;
        texts[2].setString("");
        switch_screen(SpellScreen, GameScreen, false, true);
    }
    else if (mouse_in_button(UseButton) && Game_Manager::spell_select->get_id()) {
        if (Game_Manager::spell_select->get_use() != 4) {
            const char* spell_name = Game_Manager::spell_select->get_name();
            bool success = Game_Manager::spell_use();
            if (success)
                texts[2].setString(std::format("You used {}.", spell_name));
            else
                texts[2].setString(std::format("You failed to cast {}.", spell_name));
        }
        else {
            switch_screen(SpellScreen, GameScreen, false, true);
            show_dialog(GameScreen, SpellAttackScreen);
        }
    }
    else if (mouse_in_button(DiscardButton) && Game_Manager::spell_select->get_id())
        Game_Manager::deleted_selected_sp();

    for (std::shared_ptr<Spell> spell : Game_Manager::spells) {
        if (spell->contains(x, y)) {
            // Swaps spell position
            if (Game_Manager::spell_select->get_id() && spell != Game_Manager::spell_select) {
                int sx = Game_Manager::spell_select->get_pos('x'), sy = Game_Manager::spell_select->get_pos('y');
                Game_Manager::spell_select->set_pos(spell->get_pos('x'), spell->get_pos('y'));
                spell->set_pos(sx, sy);
                Game_Manager::spell_desc = Game_Manager::spell_select = Game_Manager::placeholder_spell;
                return;
            }
            // Selects a spell
            else if (!Game_Manager::spell_select->get_id()) {
                int sx = spell->get_pos('x'), sy = spell->get_pos('y');
                map_rects["inv_sp_cur_slot"].setPosition(sx - 5, sy - 5);
                Game_Manager::spell_desc = Game_Manager::spell_select = spell;
                return;
            }
            // Unselects
            else if (spell == Game_Manager::spell_select) {
                Game_Manager::spell_desc = Game_Manager::spell_select = Game_Manager::placeholder_spell;
                return;
            }
        }
    }
}

void Spell_Screen::hover_event_handler() {
	hover_button(ExitButton);
	hover_button(UseButton);
	hover_button(DiscardButton);

	for (unsigned int i = 0; i < MAX_INV_SPELL_SLOTS; i++)
		hover_slot(i);
}

void Spell_Screen::draw() {
    window.draw(map_rects["background"]);
    if (Game_Manager::spell_select->get_id() != 0)
        window.draw(map_rects["inv_sp_cur_slot"]);
	Screen::draw();

	for (Full_Rectangle& rect : inv_sp_slots)
		window.draw(rect);

    for (std::shared_ptr<Spell> sp : Game_Manager::spells)
        sp->draw('t');

    if (Game_Manager::spell_select->get_id()) {
        window.draw(map_rects["inv_sp_use"]);
        window.draw(map_txts["inv_sp_use"]);
        window.draw(map_rects["inv_sp_discard"]);
        window.draw(map_txts["inv_sp_discard"]);
    }

	window.draw(map_rects["inv_sp_desc"]);
	window.draw(map_txts["inv_sp_desc"]);

    if (Game_Manager::spell_select->get_id())
        Game_Manager::spell_select->draw('d');

    window.draw(map_txts["inv_sp_gold"]);
    window.draw(map_txts["inv_sp_gold_amount"]);
}

void Spell_Screen::update_draw() {
	texts[1].setString(std::format("{} / {} spells",
		Game_Manager::spells.size(), Game_Manager::MAX_INV_SPELL_SLOTS));

    map_txts["inv_sp_gold_amount"].setString(std::to_string(Game_Manager::player.get_gold()));
}