/**
*
* File: stat_screen.cpp
* Description: Contain the implementation of the Stat_Screen class.
*/


#include "Screen/stat_screen.h"
#include "stat.h"
#include "Manager/game_manager.h"

Stat_Screen::Stat_Screen() : Screen(true, true, true) {
	update = true;
	setup_helper("* HP can't be less than 5.", 750.f, 250.f , NULL, NULL);
}

void Stat_Screen::click_event_handler() {
	if (mouse_in_button(ConfirmButton)) {
		switch_screen(StatScreen, GameScreen, false, true);
	}
	else if (mouse_in_button(ExitButton)) {
		Game_Manager::player.reset();
		switch_screen(StatScreen, NameScreen, false, true);
	}

	for (unsigned int i{ 0 }; i < NUM_NON_CUR_STATS * 2; i++) {
		if (!stat_curr_arrows[i].getGlobalBounds().contains(sf::Vector2f(x, y)))
			continue;

		Stat st = (Stat)(i % 6);
		long cur_stat = Game_Manager::player.get_stat(st);
		unsigned int pts_left = Game_Manager::player.get_pts();
		if (i < 6 && Game_Manager::player.get_stat(st) != 0 && !(st == Max_Hp && Game_Manager::player.get_stat(Max_Hp) == 5)) {
			if (st <= Max_Mp) Game_Manager::player.set_stat((Stat)(6 + st), Game_Manager::player.get_stat((Stat)(6 + st)) - 1);
			Game_Manager::player.set_stat(st, --cur_stat);
			stat_curr_txts[st].setString(std::to_string(cur_stat));
			Game_Manager::player.set_point(++pts_left);
			map_txts["stat_left"].setString(std::to_string(pts_left));
		}
		else if (i >= 6 && Game_Manager::player.get_pts() != 0) {
			if (st <= Max_Mp) Game_Manager::player.set_stat((Stat)(6 + st), Game_Manager::player.get_stat((Stat)(6 + st)) + 1);
			Game_Manager::player.set_stat(st, ++cur_stat);
			stat_curr_txts[st].setString(std::to_string(cur_stat));
			Game_Manager::player.set_point(--pts_left);
			map_txts["stat_left"].setString(std::to_string(pts_left));
		}
	}
}

void Stat_Screen::hover_event_handler() {
	for (unsigned int i = 0; i < NUM_NON_CUR_STATS + NUM_NON_CUR_STATS; i++) {
		bool in = stat_curr_arrows[i].getGlobalBounds().contains(sf::Vector2f(x, y));
		bool hover = stat_curr_arrows[i].getHover();

		if (in && !hover)
			stat_curr_arrows[i].highlight();
		else if (!in && hover)
			stat_curr_arrows[i].highlight(false);
	}
}

void Stat_Screen::draw() {
	Screen::draw();

	window.draw(map_txts["stat_guide"]);
	window.draw(map_txts["stat_left_guide"]);
	window.draw(map_txts["stat_left"]);

	for (Full_Text& text : stat_full_txts)
		window.draw(text);
	for (Full_Text& text : stat_curr_txts)
		window.draw(text);
	for (Full_Text& text : stat_curr_arrows)
		window.draw(text);
}

void Stat_Screen::update_draw() {
	for (unsigned int i = Max_Hp; i < NUM_NON_CUR_STATS; i++) {
		Stat st = (Stat)i;
		unsigned int cur_stat = Game_Manager::player.get_stat(st);
		stat_curr_txts[i].setString(std::to_string(cur_stat));
	}
	map_txts["stat_left"].setString(std::to_string(Game_Manager::player.get_pts()));

}

void Stat_Screen::text_event_handler() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
		switch_screen(StatScreen, GameScreen, false, true);
	}
}