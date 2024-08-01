/**
*
* File: spell_attack_screen.cpp
* Description: Contain the implementation of the Spell_Attack_Screen class.
*/

#include "Screen/spell_attack_screen.h"
#include "Manager/game_manager.h"
#include <format>

Spell_Attack_Screen::Spell_Attack_Screen() : Screen(2, 0, true, false) {
	setup_helper(0, "Spell range: ", 200.f, 10.f, NULL, 3.f);
	setup_helper(1, "0", 400, 10.f, NULL, 3.f);

	unsigned int range = sp_inf[1];
	ranges[0] = Full_Rectangle(400.f, 400.f - (40.f * range), 40.f, 40.f * range, false, true, sf::Color::Transparent, sf::Color(255, 0, 0, 100));
	ranges[1] = Full_Rectangle(440.f, 400.f, 40.f * range, 40.f, false, true, sf::Color::Transparent, sf::Color(255, 0, 0, 100));
	ranges[2] = Full_Rectangle(400.f, 440.f, 40.f, 40.f * range, false, true, sf::Color::Transparent, sf::Color(255, 0, 0, 100));
	ranges[3] = Full_Rectangle(400.f - (40.f * range), 400.f, 40.f * range, 40.f, false, true, sf::Color::Transparent, sf::Color(255, 0, 0, 100));
}

void Spell_Attack_Screen::click_event_handler() {
	if (mouse_in_button(ExitButton))
		reset_spell();

	if (sp_inf[3] > Game_Manager::player.get_stat(Mp)) {
		log_add(std::format("Insufficient MP for spell: requires {}.", sp_inf[3]).c_str());
		reset_spell();
	}
    else if (x >= 400 - (sp_inf[1] * 40) && x <= 440 + (sp_inf[1] * 40) && y >= 400 - (sp_inf[1] * 40) && y <= 440 + (sp_inf[1] * 40))
        for (unsigned int i{ 0 }; i < Game_Manager::enemies.size(); i++) {
            if (Game_Manager::enemies[i].contains(world_x, world_y)) {
                Game_Manager::pl_sp_atk(i, sp_inf);
				reset_spell();
				Game_Manager::handle_turn();
                return;
            }
        }
}

void Spell_Attack_Screen::hover_event_handler() {
	if (Game_Manager::spell_select->get_id() && sp_inf[1] == 0) {
		sp_inf = Game_Manager::spell_select->atk();
		change_range();
	}

	hover_button(ExitButton);
}

void Spell_Attack_Screen::draw() {
	Screen::draw();

	if (sp_inf[1] != 0)
		for (Full_Rectangle& rect : ranges)
			window.draw(rect);
}

void Spell_Attack_Screen::change_range() {
	unsigned int range = sp_inf[1];
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

void Spell_Attack_Screen::reset_spell() {
	Game_Manager::spell_desc = Game_Manager::spell_select = Game_Manager::placeholder_spell;
	sp_inf = { 0, 0, 0, 0 };
	return_to_prev_screen(SpellAttackScreen);
}
