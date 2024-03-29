/**
*
* File: level_screen.cpp
* Description: Contain the implementation of the Level_Screen class.
*/


#include "Screen/level_screen.h"
#include "Manager/game_manager.h"

Level_Screen::Level_Screen() : Screen(7, 1, true, true, true) {
	update = true;
	setup_helper(false, 0,           "Level:", 800.f, 190.f, NULL, NULL);
	setup_helper(false, 1,     "Current EXP:", 800.f, 240.f, NULL, NULL);
	setup_helper(false, 2, "EXP to level up:", 800.f, 290.f, NULL, NULL);

	setup_helper(false, 3, "0", 1000.f, 190.f, NULL, NULL);
	setup_helper(false, 4, "0", 1000.f, 240.f, NULL, NULL);
	setup_helper(false, 5, "0", 1000.f, 290.f, NULL, NULL);
}

void Level_Screen::click_event_handler() {
	if (mouse_in_button(ConfirmButton)) {
		reset = true;
		switch_screen(LevelScreen, GameScreen, false, true);
	}
	else if (mouse_in_button(ExitButton)) {
		for (Stat i = Max_Hp; i < Hp; i++)
			Game_Manager::player.set_stat(i, backup_stats[i]);
		Game_Manager::player.set_point(backup_pts);
		stat_left_pts.setString(std::to_string(backup_pts));
		reset = true;
		switch_screen(LevelScreen, GameScreen, false, true);
	}
	else if (mouse_in_button(ClearButton)) {
		for (Stat i = Max_Hp; i < Hp; i++)
			Game_Manager::player.set_stat(i, backup_stats[i]);
		Game_Manager::player.set_point(backup_pts);
		stat_left_pts.setString(std::to_string(backup_pts));
		reset = true;
	}

	for (Stat i{ Max_Hp }; i < Hp; i++) {
		bool in = stat_curr_arrows[i + NUM_NON_CUR_STATS].getGlobalBounds().contains(sf::Vector2f(x, y));
		unsigned int points = Game_Manager::player.get_pts();

		if (in && points != 0) {
			unsigned int stat_pt = Game_Manager::player.get_stat(i);

			Game_Manager::player.set_stat(i, ++stat_pt);
			stat_curr_txts[i].setString(std::to_string(stat_pt));
			Game_Manager::player.set_point(--points);
			stat_left_pts.setString(std::to_string(points));
		}
	}
}

void Level_Screen::hover_event_handler() {
	for (unsigned int i = NUM_NON_CUR_STATS; i < NUM_NON_CUR_STATS * 2; i++) {
		bool in = stat_curr_arrows[i].getGlobalBounds().contains(sf::Vector2f(x, y));
		bool hover = stat_curr_arrows[i].getHover();

		if (in && !hover)
			stat_curr_arrows[i].highlight();
		else if (!in && hover)
			stat_curr_arrows[i].highlight(false);
	}

	hover_button(ExitButton);
	hover_button(ConfirmButton);
	hover_button(ClearButton);
}

void Level_Screen::draw() {
	Screen::draw();

	window.draw(stat_guide_txt);
	window.draw(stat_left_txt);
	window.draw(stat_left_pts);

	window.draw(clear_rect);
	window.draw(clear_txt);

	for (Full_Text& text : stat_full_txts)
		window.draw(text);
	for (Full_Text& text : stat_curr_txts)
		window.draw(text);
	for (unsigned int i = NUM_NON_CUR_STATS; i < NUM_NON_CUR_STATS * 2; i++)
		window.draw(stat_curr_arrows[i]);
}

void Level_Screen::update_draw() {
	texts[3].setString(std::to_string(Game_Manager::player.get_lvl()));
	texts[4].setString(std::to_string(Game_Manager::player.get_cur_exp()));
	texts[5].setString(std::to_string(Game_Manager::player.get_lvl_up()));

	for (Stat i = Max_Hp; i < Hp; i++) {
		stat_curr_txts[i].setString(std::to_string(Game_Manager::player.get_stat(i)));
	}

	if (reset) {
		Game_Manager::player.copy_stat(backup_stats);
		backup_pts = Game_Manager::player.get_pts();
		reset = false;
	}
}

void Level_Screen::text_event_handler() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
		switch_screen(StatScreen, GameScreen, false, true);
	}
}