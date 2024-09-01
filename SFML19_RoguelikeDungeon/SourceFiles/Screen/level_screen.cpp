/**
*
* File: level_screen.cpp
* Description: Contain the implementation of the Level_Screen class.
*/


#include "Manager/game_manager.h"
#include "Screen/level_screen.h"
#include <array>
#include <Screen/screen.h>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <Shape/full_text.h>
#include <stat.h>
#include <string>

std::array<long, StatConst::NUM_STATS> Level_Screen::backup_stats = { 0, 0, 0, 0, 0, 0, 0, 0 };
unsigned int Level_Screen::backup_pts = 0;
bool Level_Screen::reset = true;

Level_Screen::Level_Screen() : Screen(true, true, true, true) {
	update = true;
	textRectH(          "Level:", 800.f, 190.f, NULL, NULL);
	textRectH(    "Current EXP:", 800.f, 240.f, NULL, NULL);
	textRectH("EXP to level up:", 800.f, 290.f, NULL, NULL);

	textRectH("0", 1000.f, 190.f, NULL, NULL);
	textRectH("0", 1000.f, 240.f, NULL, NULL);
	textRectH("0", 1000.f, 290.f, NULL, NULL);
}

bool Level_Screen::handleClickEvent() {
	if (mouseInButton(ConfirmButton)) {
		reset = true;
		switchScreen(LevelScreen, GameScreen, false, true);
		return true;
	}
	else if (mouseInButton(ExitButton)) {
		for (Stat i = Max_Hp; i < Hp; i++)
			Game_Manager::player.setStat(i, backup_stats[i]);
		Game_Manager::player.setStatPoint(backup_pts);
		map_txts["stat_left"].setString(std::to_string(backup_pts));
		reset = true;
		switchScreen(LevelScreen, GameScreen, false, true);
		return true;
	}
	else if (mouseInButton(ClearButton)) {
		for (Stat i = Max_Hp; i < Hp; i++)
			Game_Manager::player.setStat(i, backup_stats[i]);
		Game_Manager::player.setStatPoint(backup_pts);
		map_txts["stat_left"].setString(std::to_string(backup_pts));
		reset = true;
		return true;
	}

	for (Stat i{ Max_Hp }; i < Hp; i++) {
		bool in = stat_curr_arrows[i + NUM_NON_CUR_STATS].getGlobalBounds().contains(sf::Vector2f(x, y));
		unsigned int points = Game_Manager::player.getStatPts();

		if (in && points != 0) {
			unsigned int stat_pt = Game_Manager::player.getStat(i);

			Game_Manager::player.setStat(i, ++stat_pt);
			stat_curr_txts[i].setString(std::to_string(stat_pt));
			Game_Manager::player.setStatPoint(--points);
			map_txts["stat_left"].setString(std::to_string(points));
			return true;
		}
	}
}

void Level_Screen::handleHoverEvent() {
	for (unsigned int i = NUM_NON_CUR_STATS; i < NUM_NON_CUR_STATS * 2; i++) {
		bool in = stat_curr_arrows[i].getGlobalBounds().contains(sf::Vector2f(x, y));
		bool hover = stat_curr_arrows[i].getHover();

		if (in && !hover)
			stat_curr_arrows[i].highlight();
		else if (!in && hover)
			stat_curr_arrows[i].highlight(false);
	}
}

void Level_Screen::draw() {
	Screen::draw();

	window.draw(map_txts["stat_guide"]);
	window.draw(map_txts["stat_left_guide"]);
	window.draw(map_txts["stat_left"]);

	for (Full_Text& text : stat_full_txts)
		window.draw(text);
	for (Full_Text& text : stat_curr_txts)
		window.draw(text);
	for (unsigned int i = NUM_NON_CUR_STATS; i < NUM_NON_CUR_STATS * 2; i++)
		window.draw(stat_curr_arrows[i]);
}

void Level_Screen::updateDraw() {
	texts[3].setString(std::to_string(Game_Manager::player.getLVL()));
	texts[4].setString(std::to_string(Game_Manager::player.getCurEXP()));
	texts[5].setString(std::to_string(Game_Manager::player.getLVLUpEXP()));

	for (Stat i = Max_Hp; i < Hp; i++) {
		stat_curr_txts[i].setString(std::to_string(Game_Manager::player.getStat(i)));
	}

	if (reset) {
		Game_Manager::player.copyStat(backup_stats);
		backup_pts = Game_Manager::player.getStatPts();
		reset = false;
	}
}

void Level_Screen::handleKeyEvent() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
		switchScreen(StatScreen, GameScreen, false, true);
	}
}