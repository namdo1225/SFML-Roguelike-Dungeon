/**
*
* File: stat_screen.cpp
* Description: Contain the implementation of the Stat_Screen class.
* 
*/

#include "Manager/game_manager.h"
#include "Screen/stat_screen.h"
#include "stat.h"
#include <Screen/screen.h>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <Shape/full_text.h>
#include <string>
#include <env.h>

Stat_Screen::Stat_Screen() : Screen(true, true, true, true) {
	update = true;
	textRectH("* HP can't be less than 5.", 750.f, 250.f , NULL, NULL);
}

bool Stat_Screen::handleClickEvent() {
	if (mouseInButton(ConfirmButton)) {
		switchScreen(StatScreen, GameScreen, false, true);
		return true;
	}
	else if (mouseInButton(ExitButton)) {
		Game_Manager::player.reset();
		switchScreen(StatScreen, NameScreen, false, true);
		return true;
	}
	else if (mouseInButton(ClearButton)) {
		Game_Manager::player.reset(Game_Manager::player.getGold() > STARTING_GOLD, false);
		return true;
	}

	for (unsigned int i{ 0 }; i < NUM_NON_CUR_STATS * 2; i++) {
		if (!stat_curr_arrows[i].getGlobalBounds().contains(sf::Vector2f(x, y)))
			continue;

		Stat st = (Stat)(i % 6);
		long cur_stat = Game_Manager::player.getStat(st);
		unsigned int pts_left = Game_Manager::player.getStatPts();
		if (i < 6 && Game_Manager::player.getStat(st) != 0 && !(st == Max_Hp && Game_Manager::player.getStat(Max_Hp) == 5)) {
			if (st <= Max_Mp) Game_Manager::player.setStat((Stat)(6 + st), Game_Manager::player.getStat((Stat)(6 + st)) - 1);
			Game_Manager::player.setStat(st, --cur_stat);
			stat_curr_txts[st].setString(std::to_string(cur_stat));
			Game_Manager::player.setStatPoint(++pts_left);
			map_txts["stat_left"].setString(std::to_string(pts_left));
			return true;
		}
		else if (i >= 6 && Game_Manager::player.getStatPts() != 0) {
			if (st <= Max_Mp) Game_Manager::player.setStat((Stat)(6 + st), Game_Manager::player.getStat((Stat)(6 + st)) + 1);
			Game_Manager::player.setStat(st, ++cur_stat);
			stat_curr_txts[st].setString(std::to_string(cur_stat));
			Game_Manager::player.setStatPoint(--pts_left);
			map_txts["stat_left"].setString(std::to_string(pts_left));
			return true;
		}
	}
}

void Stat_Screen::handleHoverEvent() {
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

void Stat_Screen::updateDraw() {
	for (unsigned int i = Max_Hp; i < NUM_NON_CUR_STATS; i++) {
		Stat st = (Stat)i;
		unsigned int cur_stat = Game_Manager::player.getStat(st);
		stat_curr_txts[i].setString(std::to_string(cur_stat));
	}
	map_txts["stat_left"].setString(std::to_string(Game_Manager::player.getStatPts()));
}

void Stat_Screen::handleKeyEvent() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
		switchScreen(StatScreen, GameScreen, false, true);
	}
}