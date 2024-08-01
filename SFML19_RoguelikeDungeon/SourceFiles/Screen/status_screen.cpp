/**
*
* File: status_screen.cpp
* Description: Contain the implementation of the Status_Screen class.
*/

#include "Screen/status_screen.h"
#include "interface.h"
#include "Manager/game_manager.h"
#include <format>

sf::View Status_Screen::viewEffect{ sf::FloatRect(0, 0, 1200, 800) };

Status_Screen::Status_Screen() : Screen(true, false) {
	update = true;
	setup_helper("Status", 100.f, 10.f, 48.f, NULL);

	setup_helper("Name:", 30.f, 150.f, NULL, NULL);
	setup_helper("Level:", 30.f, 200.f, NULL, NULL);
	setup_helper("Current EXP:", 30.f, 250.f, NULL, NULL);
	setup_helper("Level up EXP:", 30.f, 300.f, NULL, NULL);
	setup_helper("Items:", 30.f, 350.f, NULL, NULL);
	setup_helper("Spells:", 30.f, 400.f, NULL, NULL);
	setup_helper("Floor:", 30.f, 450.f, NULL, NULL);
	setup_helper("Gold:", 30.f, 500.f, NULL, NULL);
	setup_helper("Stat points:", 30.f, 550.f, NULL, NULL);
	setup_helper("Weapon:", 30.f, 600.f, NULL, NULL);
	setup_helper("Armor:", 30.f, 650.f, NULL, NULL);

	setup_helper("Effects:", 600.f, 150.f, NULL, NULL);

	// Stats
	for (unsigned int i = 0; i < NUM_NON_CUR_STATS; i++) {
		setup_helper(FULL_STATS[i], 400.f, 150.f + i * 50.f, NULL, NULL);
	}

	// changing stats:
	for (unsigned int i = 0; i < 11; i++) {
		setup_helper("", 200.f, 150.f + i * 50.f, NULL, NULL);
	}

	// effects
	setup_helper("", 720.f, 150.f, NULL, NULL);

    // stats
	for (unsigned int i = 0; i < NUM_NON_CUR_STATS; i++)
		setup_helper("", 550.f, 150.f + i * 50.f, NULL, NULL);
}

void Status_Screen::click_event_handler() {
	if (mouse_in_button(ExitButton)) {
		viewEffect.reset(sf::FloatRect(0, 0, 1200, 800));
		switch_screen(StatusScreen, GameScreen, false, true);
	}
}

void Status_Screen::hover_event_handler() {
	hover_button(ExitButton);
}

void Status_Screen::update_draw() {
	// name
	texts[19].setString(Game_Manager::player.get_name());
	// level
	texts[20].setString(std::to_string(Game_Manager::player.get_lvl()));
	// current
	texts[21].setString(std::to_string(Game_Manager::player.get_cur_exp()));
	// level up EXP
	texts[22].setString(std::to_string(Game_Manager::player.get_lvl_up()));
	// items
	texts[23].setString(std::format("{} / {} items",
		Game_Manager::items.size(),
		Game_Manager::player.get_max_itm()));
	// spells
	texts[24].setString(std::format("{} / {} spells",
		Game_Manager::spells.size(), Game_Manager::MAX_INV_SPELL_SLOTS));
	// floor
	texts[25].setString(std::to_string(Game_Manager::player.get_floor()));
	// gold
	texts[26].setString(std::to_string(Game_Manager::player.get_gold()));
	// stat points
	texts[27].setString(std::to_string(Game_Manager::player.get_pts()));
	// weapon
	texts[28].setString(Game_Manager::pl_weapon->get_name());
	// armor
	texts[29].setString(Game_Manager::pl_armor->get_name());
	// effects
	std::string effects_str = "";
	const std::vector<Effect> effects = Game_Manager::player.get_effects();
	for (const Effect& effect : Game_Manager::player.get_effects()) {
		effects_str.append(std::to_string(effect.stat_difference) +
			" to " +
			Game_Manager::FULL_STATS[effect.stat_changed % NUM_NON_CUR_STATS] +
			" for " +
			std::to_string(effect.change_turns) +
			" turns.\n");
	}
	effect_txt.setString(effects_str);
	texts[30].setString(std::to_string(effects.size()));
	// stats
	for (unsigned int i = 0; i < NUM_NON_CUR_STATS; i++)
		texts[i + 31].setString(std::to_string(Game_Manager::player.get_stat((Stat)i)));
}

void Status_Screen::draw() {
	window.draw(map_rects["background"]);
	Screen::draw();

	window.setView(viewEffect);
	window.draw(effect_txt);
	window.setView(viewUI);
}

void Status_Screen::mouse_event_handler() {
	if (event.type == sf::Event::MouseWheelScrolled) {
		const float delta = event.mouseWheelScroll.delta;
		viewEffect.move(sf::Vector2f(0.f, delta * 5));
	}
}

void Status_Screen::change_theme() {
	effect_txt.flip_theme();
}