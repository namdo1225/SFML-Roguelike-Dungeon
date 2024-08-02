/**
*
* File: game_screen.cpp
* Description: Contain the implementation of the Game_Screen class.
* 
*/

#include "Screen/game_screen.h"
#include "interface.h"
#include "Manager/game_manager.h"
#include <format>

Game_Screen::Game_Screen() : Screen(false, false) {
	update = true;

	// items
	setup_helper(NULL, 750.f, 250.f, 200.f, 200.f);
	// spells
	setup_helper(NULL, 970.f, 250.f, 200.f, 200.f);
	// history
	setup_helper(NULL, 750.f, 500.f, 425.f, 275.f);
	// health/mana
	setup_helper(NULL, 950.f, 5.f, 230.f, 80.f);
	// level
	setup_helper(NULL, 950.f, 100.f, 230.f, 100.f);
	// use item shortcut
	setup_helper(NULL,  825.f, 320.f, 50.f, 50.f);
	// use spell shortcut
	setup_helper(NULL, 1045.f, 320.f, 50.f, 50.f);
	// grid button
	setup_helper(NULL,   10.f, 730.f, 50.f, 50.f);
	// opacity button
	setup_helper(NULL, 80.f, 730.f, 50.f, 50.f);
	for (unsigned int i = 0; i < 50; i++) {
		grids[i] = i < 20 ? Full_Rectangle(i * 40.f, -10.f, 0.f, 900.f, false, true, sf::Color::Black, sf::Color::Black)
			: Full_Rectangle(-10, (i - 20) * 40.f, 770.f, 0.f, false, true, sf::Color::Black, sf::Color::Black);
		grids[i].setOutlineThickness(1.f);
	}
	// range button
	setup_helper(NULL, 150.f, 730.f, 50.f, 50.f);
	// top, right, bottom, left
	unsigned int range = Game_Manager::pl_weapon->get_range();
	ranges[0] = Full_Rectangle(400.f, 400.f - (40.f * range), 40.f, 40.f * range, false, true, sf::Color::Transparent, sf::Color(255, 0, 0, 100));
	ranges[1] = Full_Rectangle(440.f, 400.f, 40.f * range, 40.f, false, true, sf::Color::Transparent, sf::Color(255, 0, 0, 100));
	ranges[2] = Full_Rectangle(400.f, 440.f, 40.f, 40.f * range, false, true, sf::Color::Transparent, sf::Color(255, 0, 0, 100));
	ranges[3] = Full_Rectangle(400.f - (40.f * range), 400.f, 40.f * range, 40.f, false, true, sf::Color::Transparent, sf::Color(255, 0, 0, 100));
	// scan button
	setup_helper(NULL, 220.f, 730.f, 50.f, 50.f);

	setup_helper("Name:", 740.f, 5.f, 18.f, 3.f);
	setup_helper(FULL_STATS[0], 960.f, 10.f, NULL, 3.f);
	setup_helper(FULL_STATS[1], 960.f, 50.f, NULL, 3.f);

	setup_helper(   "Items:", 740.f, 210.f, NULL, 3.f);
	setup_helper(  "Spells:", 950.f, 210.f, NULL, 3.f);
	setup_helper( "History:", 740.f, 455.f, NULL, 3.f);
	setup_helper(   "Level:", 960.f, 100.f, NULL, 3.f);
	setup_helper(     "Exp:", 960.f, 140.f, NULL, 3.f);
	setup_helper(   "Floor:", 10.f, 10.f, NULL, 3.f);
	setup_helper(    "Gold:", 10.f, 50.f, NULL, 3.f);
	setup_helper(   "Stats:", 740.f, 55.f, 18.f, 3.f);

	setup_helper("<", 765.f, 335.f, NULL, NULL);
	setup_helper(">", 925.f, 335.f, NULL, NULL);
	setup_helper("<", 985.f, 335.f, NULL, NULL);
	setup_helper(">", 1145.f, 335.f, NULL, NULL);

	setup_helper("G",   25.f, 740.f, 22, NULL);
	setup_helper("O",   95.f, 740.f, 22, NULL);
	setup_helper("R",  165.f, 740.f, 22, NULL);
	setup_helper("S",  235.f, 740.f, 22, NULL);

	// menu
	setupTextbox("...", 1145.f, 745.f, 50.f, 50.f, []() {
		show_dialog(Screen::display, MenuScreen);
	}, 26.0f);
	// log button
	setupTextbox("...", 1150.f, 500.f, 25.f, 25.f, []() {
		log_view(true);
		show_dialog(Screen::display, LogScreen);
	}, 5.0f);

	setup_helper(FULL_STATS[2], 750.f, 80.f, 18, 3.f);
	setup_helper(FULL_STATS[3], 750.f, 110.f, 18, 3.f);
	setup_helper(FULL_STATS[4], 750.f, 140.f, 18, 3.f);
	setup_helper(FULL_STATS[5], 750.f, 170.f, 18, 3.f);

	// texts that can be changed:
	// floor num
	setup_helper("1", 100.f, 10.f, NULL, 3.f);
	// gold num
	setup_helper("0", 100.f, 50.f, NULL, 3.f);
	// level
	setup_helper("1", 1040.f, 100.f, NULL, 3.f);
	// level exp
	setup_helper("0 / 10", 1040.f, 140.f, NULL, 3.f);
	// name
	setup_helper("Player", 740.f, 30.f, 16.f, 3.f);
	// log limit
	setup_helper("0 / 50", 850.f, 455.f, NULL, 3.f);
	// stats: hp, mp
	setup_helper("10 / 10", 1060.f, 10.f, NULL, 3.f);
	setup_helper("5 / 5", 1060.f, 50.f, NULL, 3.f);
	// other stats
	setup_helper("0", 890.f,  75.f, 18.f, 3.f);
	setup_helper("0", 890.f, 105.f, 18.f, 3.f);
	setup_helper("0", 890.f, 135.f, 18.f, 3.f);
	setup_helper("0", 890.f, 165.f, 18.f, 3.f);

	for (unsigned int i = 0; i < 5; i++)
		rects[i].setThemeAndHover(false, true);
	for (unsigned int i = 5; i < rects.size(); i++)
		rects[i].setThemeAndHover(true, true);

	for (unsigned int i = 0; i < 11; i++)
		texts[i].setThemeAndHover(true, true);
	for (unsigned int i = 11; i < 19; i++)
		texts[i].setThemeAndHover(true, true);
	for (unsigned int i = 19; i < 35; i++)
		texts[i].setThemeAndHover(false, true);

	rects[4].setThemeAndHover(false, true);
	texts[6].setThemeAndHover(false, true);
	texts[7].setThemeAndHover(false, true);
	texts[25].setThemeAndHover(false, true);
	texts[26].setThemeAndHover(false, true);
}

void Game_Screen::click_event_handler() {		
	if (mouse_in_helper(true, 4)) {
		map_txts["stat_left"].setString(std::to_string(Game_Manager::player.get_pts()));
		switch_screen(GameScreen, LevelScreen, false, true);
	}
	// use item in shortcut
	else if (mouse_in_helper(true, 7) && Game_Manager::inv_select->get_id())
		Game_Manager::item_use();
	// select new item
	else if (mouse_in_helper(true, 7) && !Game_Manager::inv_select->get_id())
		Game_Manager::itm_select_shortcut('s');
	// selects left item
	else if (mouse_in_helper(false, 11) && Game_Manager::items.size() > 1)
		Game_Manager::itm_select_shortcut('l');
	// selects right item
	else if (mouse_in_helper(false, 12) && Game_Manager::items.size() > 1)
		Game_Manager::itm_select_shortcut('r');
	// use spell in shortcut
	else if (mouse_in_helper(true, 8) && Game_Manager::spell_select->get_id()) {
		if (Game_Manager::spell_select->get_use() != 4) {
			const char* spell_name = Game_Manager::spell_select->get_name();
			bool success = Game_Manager::spell_use();
			Game_Manager::log_add(success ? std::format("You used {}.", spell_name).c_str() :
				std::format("You failed to cast {}.", spell_name).c_str()
			);
		}
		else
			show_dialog(GameScreen, SpellAttackScreen);
	}
	// select new spell
	else if (mouse_in_helper(true, 8) && !Game_Manager::spell_select->get_id())
		Game_Manager::sp_select_shortcut('s');
	// selects left spell
	else if (mouse_in_helper(false, 13) && Game_Manager::spells.size() > 1)
		Game_Manager::sp_select_shortcut('l');
	// selects right spell
	else if (mouse_in_helper(false, 14) && Game_Manager::spells.size() > 1)
		Game_Manager::sp_select_shortcut('r');
	else if (x >= 400 && x <= 440 && y >= 360 - ((Game_Manager::pl_weapon->get_range() - 1) * 40) && y <= 400)
		Game_Manager::handle_player_action('u', 1);
	else if (x >= 440 && x <= 480 + ((Game_Manager::pl_weapon->get_range() - 1) * 40) && y >= 400 && y <= 440)
		Game_Manager::handle_player_action('r', 1);
	else if (x >= 400 && x <= 440 && y >= 440 && y <= 480 + ((Game_Manager::pl_weapon->get_range() - 1) * 40))
		Game_Manager::handle_player_action('d', 1);
	else if (x >= 360 - ((Game_Manager::pl_weapon->get_range() - 1) * 40) && x <= 400 && y >= 400 && y <= 440)
		Game_Manager::handle_player_action('l', 1);
	else if (mouse_in_helper(true, 9))
		grid = !grid;
	else if (mouse_in_helper(true, 10))
		change_opacity();
	else if (mouse_in_helper(true, 11)) {
		range = !range;
		if (range) change_range();
	}
	else if (mouse_in_helper(true, 12))
		scan = !scan;
}

void Game_Screen::hover_event_handler() {
	hover_textbox( 5, -1);
	hover_textbox( 6, -1);
	hover_textbox( 7, 15);
	hover_textbox( 8, 16);
	hover_textbox( 9, 17);
	hover_textbox(10, 18);

	hover_helper(false, 11);
	hover_helper(false, 12);

	hover_helper(false, 13);
	hover_helper(false, 14);

	hover_helper(true, 4);
	hover_helper(false, 6);
	hover_helper(false, 7);
	hover_helper(false, 27);
	hover_helper(false, 28);

	if (scan) {
		bool enemy_found = false;
		for (Enemy& enemy : Game_Manager::enemies) {
			if (enemy.contains(world_x, world_y)) {
				enemy_found = true;
				scan_txt.setString(std::format(
					"{}\nHP: {}"
					"\nATK: {}"
					"\nDEF: {}"
					"\nRES: {}"
					"\nRAG: {}",
					enemy.get_name(), enemy.get_stat(0), enemy.get_stat(1), enemy.get_stat(2), enemy.get_stat(3), enemy.get_stat(4)));
				break;
			}
		}
		if (!enemy_found)
			scan_txt.setString("Hover\nover\nenemies\nfor their\nstats.");
	}
}

void Game_Screen::draw() {
	window.draw(map_rects["background"]);
	window.setView(viewWorld);

	Game_Manager::floor.draw();
	window.draw(Game_Manager::player);
	for (Enemy en : Game_Manager::enemies)
		window.draw(en);

	window.setView(viewUI);

	if (grid)
		for (Full_Rectangle& rect : grids)
			window.draw(rect);

	if (range)
		for (Full_Rectangle& rect : ranges)
			window.draw(rect);

	if (scan) {
		window.draw(scan_rect);
		window.draw(scan_txt);
	}

	Screen::draw();
	for (unsigned int i = logs.size() < 8 ? 0 : (logs.size() - 8); i < logs.size(); i++)
		window.draw(logs[i]);

	if (Game_Manager::spell_select->get_id())
		Game_Manager::spell_select->draw('s');

	if (Game_Manager::inv_select->get_id())
		Game_Manager::inv_select->draw('s');
}

void Game_Screen::key_event_handler() {
	if (!Game_Manager::player.is_stuck(0) && event.key.code == sf::Keyboard::Up) {
		Game_Manager::handle_player_action('u', 0);
		window.setView(viewWorld);
	}
	else if (!Game_Manager::player.is_stuck(1) && event.key.code == sf::Keyboard::Right) {
		Game_Manager::handle_player_action('r', 0);
		window.setView(viewWorld);
	}
	else if (!Game_Manager::player.is_stuck(2) && event.key.code == sf::Keyboard::Down) {
		Game_Manager::handle_player_action('d', 0);
		window.setView(viewWorld);
	}
	else if (!Game_Manager::player.is_stuck(3) && event.key.code == sf::Keyboard::Left) {
		Game_Manager::handle_player_action('l', 0);
		window.setView(viewWorld);
	}
	else if (event.key.code == sf::Keyboard::W) {
		Game_Manager::handle_turn();
		window.setView(viewWorld);
	}
	else if (event.key.code == sf::Keyboard::Q) {
		Game_Manager::next_level();
		window.setView(viewWorld);
	}
	else if (event.key.code == sf::Keyboard::G)
		grid = !grid;
	else if (event.key.code == sf::Keyboard::O)
		change_opacity();
	else if (event.key.code == sf::Keyboard::R)
		change_range();
	else if (event.key.code == sf::Keyboard::S && Game_Manager::floor.shop_intersect(Game_Manager::player.get_rect())) {
		switch_screen(GameScreen, ShopScreen, false, true);
	}
}

void Game_Screen::update_draw() {
	if (Game_Manager::game_over()) {
		switch_screen(GameScreen, TitleScreen, true);
		return;
	}

	texts[28].setString(std::format("{} / 50", logs.size()));

	texts[23].setString(std::to_string(Game_Manager::player.get_floor()));
	texts[24].setString(std::to_string(Game_Manager::player.get_gold()));
	texts[25].setString(std::to_string(Game_Manager::player.get_lvl()));
	texts[26].setString(std::format("{} / {}", Game_Manager::player.get_cur_exp(), Game_Manager::player.get_lvl_up()));
	texts[27].setString(Game_Manager::player.get_name());

	texts[29].setString(std::format("{} / {}", Game_Manager::player.get_stat(Hp), Game_Manager::player.get_stat(Max_Hp)));
	texts[30].setString(std::format("{} / {}", Game_Manager::player.get_stat(Mp), Game_Manager::player.get_stat(Max_Mp)));

	texts[31].setString(std::to_string(Game_Manager::player.get_stat(Str)));
	texts[32].setString(std::to_string(Game_Manager::player.get_stat(Def)));
	texts[33].setString(std::to_string(Game_Manager::player.get_stat(Mgk)));
	texts[34].setString(std::to_string(Game_Manager::player.get_stat(Res)));

	if (range)
		change_range();
}

void Game_Screen::change_theme() {
	scan_rect.flip_theme();
	scan_txt.flip_theme();
	scan_txt.setFont(Font_Manager::get_selected());

	for (std::shared_ptr<Item> item : Game_Manager::items)
		item->change_theme();
	for (std::shared_ptr<Item> item : Game_Manager::item_stocks)
		item->change_theme();

	for (std::shared_ptr<Spell> spell : Game_Manager::spells)
		spell->change_theme();
	for (std::shared_ptr<Spell> spell : Game_Manager::spell_stocks)
		spell->change_theme();

	for (std::shared_ptr<Special> special : Game_Manager::special_stocks)
		special->change_theme();
}

void Game_Screen::change_opacity() {
	opacity = (opacity + 17) % 255;
	if (opacity == 0)
		opacity = 255;

	for (Full_Rectangle& rect : rects) {
		sf::Color fill = rect.getFillColor();
		sf::Color outline = rect.getOutlineColor();

		rect.setFillColor(sf::Color(fill.r, fill.g, fill.b, opacity));
		rect.setOutlineColor(sf::Color(outline.r, outline.g, outline.b, opacity));
	}

	for (Full_Text& text : texts) {
		sf::Color fill = text.getFillColor();
		sf::Color outline = text.getOutlineColor();

		text.setFillColor(sf::Color(fill.r, fill.g, fill.b, opacity));
		text.setOutlineColor(sf::Color(outline.r, outline.g, outline.b, opacity));
	}

	for (Full_Text& text : logs) {
		sf::Color fill = text.getFillColor();
		sf::Color outline = text.getOutlineColor();

		text.setFillColor(sf::Color(fill.r, fill.g, fill.b, opacity));
		text.setOutlineColor(sf::Color(outline.r, outline.g, outline.b, opacity));
	}
}

void Game_Screen::change_range() {
	unsigned int range = Game_Manager::pl_weapon->get_range();
	ranges[0].setPosition(400.f, 400.f - (40.f * range));
	ranges[0].setSize(sf::Vector2f(40.f, 40.f * range));

	ranges[1].setPosition(440.f, 400.f);
	ranges[1].setSize(sf::Vector2f(40.f * range, 40.f));

	ranges[2].setPosition(400.f, 440.f);
	ranges[2].setSize(sf::Vector2f(40.f, 40.f * range));

	ranges[3].setPosition(400.f - (40.f * range), 400.f);
	ranges[3].setSize(sf::Vector2f(40.f * range, 40.f));
}
