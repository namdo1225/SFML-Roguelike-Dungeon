/**
*
* File: game_screen.cpp
* Description: Contain the implementation of the Game_Screen class.
* 
*/

#include "Manager/game_manager.h"
#include "Screen/game_screen.h"
#include <Candle/LightingArea.hpp>
#include <Candle/RadialLight.hpp>
#include <Floor/enemy.h>
#include <format>
#include <Manager/setting_manager.h>
#include <memory>
#include <Screen/screen.h>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <Shape/full_rectangle.h>
#include <Shape/full_text.h>
#include <Shape/full_textbox.h>
#include <stat.h>
#include <string>
#include <Tool/item.h>
#include <Tool/special.h>
#include <Tool/spell.h>

bool Game_Screen::grid = false;
bool Game_Screen::range = false;
bool Game_Screen::scan = false;
unsigned int Game_Screen::opacity = 255;

Full_Rectangle Game_Screen::grids[50];
Full_Rectangle Game_Screen::ranges[4];

Full_Rectangle Game_Screen::scan_rect = Full_Rectangle(10.f, 530.f, 100.f, 160.f, true, true);
Full_Text Game_Screen::scan_txt = Full_Text(20.f, 540.f, 18.f, "", true, true);

candle::LightingArea Game_Screen::fog = candle::LightingArea(candle::LightingArea::FOG,
	sf::Vector2f(0.f, 0.f),
	sf::Vector2f(DEFAULT_SCREEN_X, DEFAULT_SCREEN_Y));
candle::RadialLight Game_Screen::light;

sf::Clock Game_Screen::clock;
const float Game_Screen::KEY_PRESS_TIME_LIMIT = 250.f;

Game_Screen::Game_Screen() : Screen(false, false) {
	update = true;
	fog.setAreaColor(sf::Color::Black);
	light.setRange(300);
	light.setFade(true);
	light.setPosition(sf::Vector2f(620.f, 420.f));

	for (unsigned int i = 0; i < 50; i++) {
		grids[i] = i < 20 ? Full_Rectangle(i * 40.f, -10.f, 0.f, 900.f, false, true, sf::Color::Black, sf::Color::Black)
			: Full_Rectangle(-10, (i - 20) * 40.f, 770.f, 0.f, false, true, sf::Color::Black, sf::Color::Black);
		grids[i].setOutlineThickness(1.f);
	}
	// top, right, bottom, left
	ranges[0] = Full_Rectangle(600.f, 400.f, 40.f, 40.f, false, true, sf::Color::Transparent, sf::Color(255, 0, 0, 100));
	ranges[1] = Full_Rectangle(640.f, 400.f, 40.f, 40.f, false, true, sf::Color::Transparent, sf::Color(255, 0, 0, 100));
	ranges[2] = Full_Rectangle(600.f, 440.f, 40.f, 40.f, false, true, sf::Color::Transparent, sf::Color(255, 0, 0, 100));
	ranges[3] = Full_Rectangle(600.f, 400.f, 40.f, 40.f, false, true, sf::Color::Transparent, sf::Color(255, 0, 0, 100));

	setupHoverableText("<", 985.f, 260.f, []() {
		Game_Manager::itm_select_shortcut('l');
	});
	setupHoverableText(">", 1145.f, 260.f, []() {
		Game_Manager::itm_select_shortcut('r');
	});
	setupHoverableText("<", 985.f, 380.f, []() {
		Game_Manager::sp_select_shortcut('l');
	});
	setupHoverableText(">", 1145.f, 380.f, []() {
		Game_Manager::sp_select_shortcut('r');
	});

	// level
	setupTextbox(NULL, 950.f, 100.f, 230.f, 100.f, []() {
		map_txts["stat_left"].setString(std::to_string(Game_Manager::player.get_pts()));
		switch_screen(GameScreen, LevelScreen, false, true);
		});
	// HP/MP
	setupTextbox(NULL, 950.f, 5.f, 230.f, 80.f, []() {
		switch_screen(GameScreen, StatusScreen, true);
	});

	// use item shortcut
	setupTextbox(NULL, 1045.f, 250.f, 50.f, 50.f, []() {
		// Use item
		if (Game_Manager::inv_select->id)
			Game_Manager::item_use();
		// Selects new item
		else
			Game_Manager::itm_select_shortcut('s');
		});
	// use spell shortcut
	setupTextbox(NULL, 1045.f, 370.f, 50.f, 50.f, []() {
		// use spell in shortcut
		if (Game_Manager::spell_select->id) {
			if (Game_Manager::spell_select->type == Functional) {
				std::string name = Game_Manager::spell_select->name;
				bool success = Game_Manager::spell_use();
				Game_Manager::log_add(success ? std::format("You used {}.", name).c_str() :
					std::format("You failed to cast {}.", name).c_str()
				);
			}
			else
				show_dialog(GameScreen, SpellAttackScreen);
		}
		// select new spell
		else
			Game_Manager::sp_select_shortcut('s');
		});

	// grid
	setupTextbox("G", 10.f, 730.f, 50.f, 50.f, []() {
		grid = !grid;
	});
	// opacity
	setupTextbox("O", 80.f, 730.f, 50.f, 50.f, [this]() {
		change_opacity();
	});
	// range
	setupTextbox("R", 150.f, 730.f, 50.f, 50.f, []() {
		range = !range;
		if (range) change_range();
	});
	// scan
	setupTextbox("S", 220.f, 730.f, 50.f, 50.f, []() {
		scan = !scan;
	});
	// zoom options
	setupTextbox("+", 290.f, 730.f, 50.f, 50.f, []() {
		if (worldZoomLevel < 3) {
			worldZoomLevel++;
			viewWorld.zoom(0.5f);
			Game_Manager::center_floor();
		}
	});
	setupTextbox("-", 360.f, 730.f, 50.f, 50.f, []() {
		if (worldZoomLevel > 0) {
			worldZoomLevel--;
			viewWorld.zoom(2.f);
			Game_Manager::center_floor();
		}
	});
	// menu
	setupTextbox("...", 430.f, 730.f, 50.f, 50.f, []() {
		show_dialog(Screen::display, MenuScreen);
	}, 26.0f);

	// log button
	setupTextbox("...", 1150.f, 500.f, 25.f, 25.f, []() {
		log_view(true);
		show_dialog(Screen::display, LogScreen);
	}, 5.0f);
	setupTextbox("O", 1150.f, 525.f, 25.f, 25.f, [this]() {
		sf::Color fill = rects[0].getFillColor();
		sf::Color outline = rects[0].getOutlineColor();


		rects[0].setFillColor(sf::Color(fill.r, fill.g, fill.b, fill.a ? 0 : 255));
		rects[0].setOutlineColor(sf::Color(outline.r, outline.g, outline.b, outline.a ? 0 : 255));
	}, 5.0f);

	// history
	setup_helper(NULL, 750.f, 500.f, 425.f, 275.f, false, true);

	setup_helper("Items:", 950.f, 210.f, NULL, 3.f, false, true);
	setup_helper("Spells:", 950.f, 330.f, NULL, 3.f, false, true);
	setup_helper("History:", 950.f, 455.f, NULL, 3.f, false, true);
	setup_helper("Floor:", 10.f, 50.f, NULL, 3.f, false, true);
	setup_helper("Gold:", 10.f, 90.f, NULL, 3.f, false, true);

	for (unsigned int i = 0; i < 4; i++)
		setup_helper(StatConst::ABR_STATS[i + 2], 10.f, 130 + (30 * i), 18, 3.f, false, true);

	// texts that can be changed:
	// floor num
	setup_helper("1", 100.f, 50.f, NULL, 3.f, false, true);
	// gold num
	setup_helper("0", 100.f, 90.f, NULL, 3.f, false, true);
	// name
	setup_helper("Player", 10.f, 10.f, NULL, 3.f, false, true);
	// log limit
	setup_helper("0 / 50", 1060.f, 455.f, NULL, 3.f, false, true);
	// other stats
	for (unsigned int i = 0; i < 4; i++)
		setup_helper("0", 90.f, 130 + (30 * i), 18.f, 3.f, false, true);

	for (unsigned int i = 0; i < textboxes.size(); i++) {
		textboxes[i].text.setThemeAndHover(true, true);
		textboxes[i].rect.setThemeAndHover(true, true);
	}

	for (unsigned int i = 0; i < hoverableTexts.size(); i++) {
		hoverableTexts[i].setThemeAndHover(true, true);
	}
}

bool Game_Screen::click_event_handler() {
	unsigned int rgn = Game_Manager::pl_weapon->range;
	if (x >= 600 && x <= 640 && y >= 360 - ((rgn - 1) * 40) && y <= 400) {
		Game_Manager::handle_player_action('u', 1);
		return true;
	}
	else if (x >= 640 && x <= 680 + ((rgn - 1) * 40) && y >= 400 && y <= 440) {
		Game_Manager::handle_player_action('r', 1);
		return true;
	}
	else if (x >= 600 && x <= 640 && y >= 440 && y <= 480 + ((rgn - 1) * 40)) {
		Game_Manager::handle_player_action('d', 1);
		return true;
	}
	else if (x >= 560 - ((rgn - 1) * 40) && x <= 400 && y >= 400 && y <= 440) {
		return true;
		Game_Manager::handle_player_action('l', 1);
	}
}

void Game_Screen::hover_event_handler() {
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
					enemy.constant->name, enemy.stat.hp, enemy.stat.atk, enemy.stat.def, enemy.stat.res, enemy.stat.range));
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

	fog.clear();
	fog.draw(light);
	fog.display();

	window.draw(fog);

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

	if (Game_Manager::spell_select)
		Game_Manager::spell_select->draw();

	if (Game_Manager::inv_select)
		Game_Manager::inv_select->draw();
}

void Game_Screen::key_event_handler() {
	if (clock.getElapsedTime().asMilliseconds() > KEY_PRESS_TIME_LIMIT) {
		clock.restart();
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
	}
	else if (clock.getElapsedTime().asMilliseconds() > KEY_PRESS_TIME_LIMIT * 30)
		clock.restart();

	if (event.key.code == sf::Keyboard::W) {
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

	textboxes[0].text.setString(std::format("LVL: {}\nCUR: {}\nNEED: {}",
		Game_Manager::player.get_lvl(), Game_Manager::player.get_cur_exp(), Game_Manager::player.get_lvl_up()));
	textboxes[0].recenterText();

	textboxes[1].text.setString(std::format("HP: {} / {}\nMP: {} / {}",
		Game_Manager::player.get_stat(Hp), Game_Manager::player.get_stat(Max_Hp),
		Game_Manager::player.get_stat(Mp), Game_Manager::player.get_stat(Max_Mp)));
	textboxes[1].recenterText();

	texts[9].setString(std::to_string(Game_Manager::player.get_floor()));
	texts[10].setString(std::to_string(Game_Manager::player.get_gold()));

	texts[11].setString(Game_Manager::player.get_name());

	texts[12].setString(std::format("{} / 50", logs.size()));

	texts[13].setString(std::to_string(Game_Manager::player.get_stat(Str)));
	texts[14].setString(std::to_string(Game_Manager::player.get_stat(Def)));
	texts[15].setString(std::to_string(Game_Manager::player.get_stat(Mgk)));
	texts[16].setString(std::to_string(Game_Manager::player.get_stat(Res)));
}

void Game_Screen::change_theme() {
	scan_rect.flip_theme();
	scan_txt.flip_theme();

	fog.setAreaColor(Setting_Manager::light ? Full_Rectangle::light_bg[theme] : Full_Rectangle::dark_bg[theme]);

	for (Item& item : Game_Manager::items)
		item.changeTheme();
	for (Item& item : Game_Manager::item_stocks)
		item.changeTheme();

	for (Spell& spell : Game_Manager::spells)
		spell.changeTheme();
	for (Spell& spell : Game_Manager::spell_stocks)
		spell.changeTheme();

	for (Special& special : Game_Manager::special_stocks)
		special.changeTheme();
}

void Game_Screen::change_opacity() {
	opacity = (opacity + 17) % 255;
	if (opacity == 0)
		opacity = 255;

	for (Full_Textbox& textbox : textboxes) {
		sf::Color fillRect = textbox.rect.getFillColor();
		sf::Color outlineRect = textbox.rect.getOutlineColor();

		textbox.rect.setFillColor(sf::Color(fillRect.r, fillRect.g, fillRect.b, opacity));
		textbox.rect.setOutlineColor(sf::Color(outlineRect.r, outlineRect.g, outlineRect.b, opacity));

		sf::Color fillText = textbox.text.getFillColor();
		sf::Color outlineText = textbox.text.getOutlineColor();

		textbox.text.setFillColor(sf::Color(fillText.r, fillText.g, fillText.b, opacity));
		textbox.text.setOutlineColor(sf::Color(outlineText.r, outlineText.g, outlineText.b, opacity));
	}

	for (Full_Rectangle& rect : rects) {
		sf::Color fill = rect.getFillColor();
		sf::Color outline = rect.getOutlineColor();

		rect.setFillColor(sf::Color(fill.r, fill.g, fill.b, opacity));
		rect.setOutlineColor(sf::Color(outline.r, outline.g, outline.b, opacity));
	}

	for (Full_Text& text : hoverableTexts) {
		sf::Color fill = text.getFillColor();
		sf::Color outline = text.getOutlineColor();

		text.setFillColor(sf::Color(fill.r, fill.g, fill.b, opacity));
		text.setOutlineColor(sf::Color(outline.r, outline.g, outline.b, opacity));
	}

	for (Full_Text& text : texts) {
		sf::Color fill = text.getFillColor();
		sf::Color outline = text.getOutlineColor();

		text.setFillColor(sf::Color(fill.r, fill.g, fill.b, opacity));
		text.setOutlineColor(sf::Color(outline.r, outline.g, outline.b, opacity));
	}
}

void Game_Screen::change_range() {
	unsigned int range = Game_Manager::pl_weapon->range;
	ranges[0].setPosition(600.f, 400.f - (40.f * range));
	ranges[0].setSize(sf::Vector2f(40.f, 40.f * range));

	ranges[1].setPosition(640.f, 400.f);
	ranges[1].setSize(sf::Vector2f(40.f * range, 40.f));

	ranges[2].setPosition(600.f, 440.f);
	ranges[2].setSize(sf::Vector2f(40.f, 40.f * range));

	ranges[3].setPosition(600.f - (40.f * range), 400.f);
	ranges[3].setSize(sf::Vector2f(40.f * range, 40.f));
}
