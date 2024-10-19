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
#include <Screen/screen.h>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
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
bool Game_Screen::enemyFound = false;

unsigned int Game_Screen::opacity = 255;

Full_Rectangle Game_Screen::grids[60];
Full_Rectangle Game_Screen::rangeBox;

Full_Rectangle Game_Screen::rectScan = Full_Rectangle(10.f, 530.f, 100.f, 160.f, true, true);
Full_Text Game_Screen::scanText = Full_Text(20.f, 540.f, 18.f, "", true, true);

candle::LightingArea Game_Screen::fog = candle::LightingArea(candle::LightingArea::FOG,
	sf::Vector2f(0.f, 0.f),
	sf::Vector2f(DEFAULT_SCREEN_X, DEFAULT_SCREEN_Y));
candle::RadialLight Game_Screen::light;

sf::Clock Game_Screen::clock;
const float Game_Screen::KEY_PRESS_TIME_LIMIT = 250.f;

Full_Text Game_Screen::atkIcon = Full_Text(-10.f, -10.f, 16.f, "ATK", false);

Game_Screen::Game_Screen() : Screen(false, false) {
	update = true;
	fog.setAreaColor(sf::Color::Black);
	light.setRange(300);
	light.setFade(true);
	light.setPosition(sf::Vector2f(620.f, 420.f));

	for (unsigned int i = 0; i < 60; i++) {
		grids[i] = i < 30 ? Full_Rectangle(i * 40.f, -10.f, 0.f, 900.f, false, true, sf::Color::Black, sf::Color::Black)
			: Full_Rectangle(-10, (i - 30) * 40.f, 1300.f, 0.f, false, true, sf::Color::Black, sf::Color::Black);
		grids[i].setOutlineThickness(1.f);
	}
	rangeBox = Full_Rectangle(600.f, 400.f, 40.f, 40.f, false, true, sf::Color::Transparent, sf::Color(255, 0, 0, 100));

	hoverableTextH("<", 985.f, 260.f, [this]() {
		Game_Manager::findItemShortcut('l');
		if (Game_Manager::selectedInv != -1) {
			textboxes[2].text.setString(Game_Manager::items[Game_Manager::selectedInv].getAbbrev());
			textboxes[2].recenterText();
		}
	});
	hoverableTextH(">", 1145.f, 260.f, [this]() {
		Game_Manager::findItemShortcut('r');
		if (Game_Manager::selectedInv != -1) {
			textboxes[2].text.setString(Game_Manager::items[Game_Manager::selectedInv].getAbbrev());
			textboxes[2].recenterText();
		}
	});
	hoverableTextH("<", 985.f, 380.f, [this]() {
		Game_Manager::findSpellShortcut('l');
		if (Game_Manager::selectedSpell != -1) {
			textboxes[3].text.setString(Game_Manager::spells[Game_Manager::selectedSpell].getAbbrev());
			textboxes[3].recenterText();
		}
	});
	hoverableTextH(">", 1145.f, 380.f, [this]() {
		Game_Manager::findSpellShortcut('r');
		if (Game_Manager::selectedSpell != -1) {
			textboxes[3].text.setString(Game_Manager::spells[Game_Manager::selectedSpell].getAbbrev());
			textboxes[3].recenterText();
		}
	});

	// level
	textboxH(NULL, 950.f, 100.f, 230.f, 100.f, []() {
		map_txts["stat_left"].setString(std::to_string(Game_Manager::player.getStatPts()));
		switchScreen(GameScreen, LevelScreen, false, true);
		});
	// HP/MP
	textboxH(NULL, 950.f, 5.f, 230.f, 80.f, []() {
		switchScreen(GameScreen, StatusScreen, true);
	});

	// use item shortcut
	textboxH(NULL, 1045.f, 250.f, 50.f, 50.f, [this]() {
		// Use item
		if (Game_Manager::selectedInv != -1)
			Game_Manager::useItem();
		// Selects new item
		else {
			Game_Manager::findItemShortcut('l');
			if (Game_Manager::selectedInv != -1) {
				textboxes[2].text.setString(Game_Manager::items[Game_Manager::selectedInv].getAbbrev());
				textboxes[2].recenterText();
			}
		}
	});
	// use spell shortcut
	textboxH(NULL, 1045.f, 370.f, 50.f, 50.f, [this]() {
		// use spell in shortcut
		if (Game_Manager::selectedSpell != -1) {
			if (Game_Manager::spells[Game_Manager::selectedSpell].getType() == Functional) {
				std::string name = Game_Manager::spells[Game_Manager::selectedSpell].getName();
				bool success = Game_Manager::useSpell();
				Game_Manager::log_add(success ? std::format("You used {}.", name).c_str() :
					std::format("You failed to cast {}.", name).c_str()
				);
			}
			else
				openDialog(GameScreen, SpellAttackScreen);
		}
		// select new spell
		else {
			Game_Manager::findSpellShortcut('l');
			if (Game_Manager::selectedSpell != - 1) {
				textboxes[3].text.setString(Game_Manager::spells[Game_Manager::selectedSpell].getAbbrev());
				textboxes[3].recenterText();
			}
		}
	});

	// grid
	textboxH("G", 10.f, 730.f, 50.f, 50.f, []() {
		grid = !grid;
		if (grid)
			changeGrid();
	});
	// opacity
	textboxH("O", 80.f, 730.f, 50.f, 50.f, [this]() {
		changeOpacity();
	});
	// range
	textboxH("R", 150.f, 730.f, 50.f, 50.f, []() {
		range = !range;
		if (range) changeRange();
	});
	// scan
	textboxH("S", 220.f, 730.f, 50.f, 50.f, []() {
		scan = !scan;
	});
	// zoom options
	textboxH("+", 290.f, 730.f, 50.f, 50.f, []() {
		if (worldZoomLevel < 3) {
			worldZoomLevel++;
			viewWorld.zoom(0.5f);
			Game_Manager::centerFloor();
		}
	});
	textboxH("-", 360.f, 730.f, 50.f, 50.f, []() {
		if (worldZoomLevel > 0) {
			worldZoomLevel--;
			viewWorld.zoom(2.f);
			Game_Manager::centerFloor();
		}
	});
	// menu
	textboxH("...", 430.f, 730.f, 50.f, 50.f, []() {
		openDialog(Screen::display, MenuScreen);
	}, 26.0f);

	// log button
	textboxH("...", 1150.f, 500.f, 25.f, 25.f, []() {
		log_view(true);
		openDialog(Screen::display, LogScreen);
	}, 5.0f);
	textboxH("O", 1150.f, 525.f, 25.f, 25.f, [this]() {
		sf::Color fill = rects[0].getFillColor();
		sf::Color outline = rects[0].getOutlineColor();


		rects[0].setFillColor(sf::Color(fill.r, fill.g, fill.b, fill.a ? 0 : 255));
		rects[0].setOutlineColor(sf::Color(outline.r, outline.g, outline.b, outline.a ? 0 : 255));
	}, 5.0f);

	// history
	textRectH(NULL, 750.f, 500.f, 425.f, 275.f, false, true);

	textRectH("Items:", 950.f, 210.f, NULL, 3.f, false, true);
	textRectH("Spells:", 950.f, 330.f, NULL, 3.f, false, true);
	textRectH("History:", 950.f, 455.f, NULL, 3.f, false, true);
	textRectH("Floor:", 10.f, 50.f, NULL, 3.f, false, true);
	textRectH("Gold:", 10.f, 90.f, NULL, 3.f, false, true);

	for (unsigned int i = 0; i < 4; i++)
		textRectH(StatConst::ABR_STATS[i + 2], 10.f, 130 + (30 * i), 18, 3.f, false, true);

	// texts that can be changed:
	// floor num
	textRectH("1", 100.f, 50.f, NULL, 3.f, false, true);
	// gold num
	textRectH("0", 100.f, 90.f, NULL, 3.f, false, true);
	// name
	textRectH("Player", 10.f, 10.f, NULL, 3.f, false, true);
	// log limit
	textRectH("0 / 50", 1060.f, 455.f, NULL, 3.f, false, true);
	// other stats
	for (unsigned int i = 0; i < 4; i++)
		textRectH("0", 90.f, 130 + (30 * i), 18.f, 3.f, false, true);

	for (unsigned int i = 0; i < textboxes.size(); i++) {
		textboxes[i].text.setThemeAndHover(true, true);
		textboxes[i].rect.setThemeAndHover(true, true);
	}

	for (unsigned int i = 0; i < hoverableTexts.size(); i++) {
		hoverableTexts[i].setThemeAndHover(true, true);
	}
}

bool Game_Screen::handleClickEvent() {
	unsigned int rgn = (Game_Manager::plWeapon.getRange() - 1) * 40;
	if (x >= 600 && x <= 640 && y >= 360 - rgn && y <= 400) {
		Game_Manager::handlePlayerAct(sf::Keyboard::Up, 1);
		return true;
	}
	else if (x >= 640 && x <= 680 + rgn && y >= 400 && y <= 440) {
		Game_Manager::handlePlayerAct(sf::Keyboard::Right, 1);
		return true;
	}
	else if (x >= 600 && x <= 640 && y >= 440 && y <= 480 + rgn) {
		Game_Manager::handlePlayerAct(sf::Keyboard::Down, 1);
		return true;
	}
	else if (x >= 560 - rgn && x <= 400 && y >= 400 && y <= 440) {
		return true;
		Game_Manager::handlePlayerAct(sf::Keyboard::Left, 1);
	}
}

void Game_Screen::handleHoverEvent() {
	enemyFound = false;
	for (Enemy& enemy : Game_Manager::enemies) {
		if (enemy.contains(worldX, worldY)) {
			enemyFound = true;

			if (scan) {
				scanText.setString(std::format(
					"{}\nHP: {}"
					"\nATK: {}"
					"\nDEF: {}"
					"\nRES: {}"
					"\nRAG: {}",
					enemy.constant->name, enemy.stat.hp, enemy.stat.atk, enemy.stat.def, enemy.stat.res, enemy.stat.range));
			}

			if (rangeBox.getGlobalBounds().intersects(enemy.getGlobalBounds()))
				atkIcon.setPosition(x + 15, y + 15);
			else
				atkIcon.setPosition(-10.f, -10.f);

			break;
		}
	}

	if (!enemyFound)
		scanText.setString("Hover\nover\nenemies\nfor their\nstats.");
}

void Game_Screen::draw() {
	window.draw(map_rects["background"]);
	window.setView(viewWorld);

	Game_Manager::floor.draw();
	window.draw(Game_Manager::player);
	for (Enemy& en : Game_Manager::enemies)
		window.draw(en);

	if (grid)
		for (Full_Rectangle& rect : grids)
			window.draw(rect);

	if (range)
		window.draw(rangeBox);

	window.setView(viewUI);

	fog.clear();
	fog.draw(light);
	fog.display();

	window.draw(fog);

	if (scan) {
		window.draw(rectScan);
		window.draw(scanText);
	}

	if (enemyFound)
		window.draw(atkIcon);

	Screen::draw();
	for (unsigned int i = logs.size() < 8 ? 0 : (logs.size() - 8); i < logs.size(); i++)
		window.draw(logs[i]);
}

void Game_Screen::handleKeyEvent() {
	if (clock.getElapsedTime().asMilliseconds() > KEY_PRESS_TIME_LIMIT &&
		event.key.code <= sf::Keyboard::Down && event.key.code >= sf::Keyboard::Left) {
		clock.restart();
		Game_Manager::handlePlayerAct(event.key.code, 0);
		changeRange();
		changeGrid();
	}
	else if (clock.getElapsedTime().asMilliseconds() > KEY_PRESS_TIME_LIMIT * 30)
		clock.restart();
	
	switch (event.key.code) {
	case sf::Keyboard::W:
		Game_Manager::handleTurn();
		window.setView(viewWorld);
		break;
	case sf::Keyboard::Q:
		Game_Manager::goUpFloor();
		window.setView(viewWorld);
		break;
	case sf::Keyboard::G:
		grid = !grid;
		break;
	case sf::Keyboard::O:
		changeOpacity();
		break;
	case sf::Keyboard::R:
		changeRange();
		break;
	case sf::Keyboard::S:
		if (Game_Manager::floor.intersectShop(Game_Manager::player.getRect())) {
			Game_Manager::selectedInv = Game_Manager::selectedSpecial = Game_Manager::selectedSpell = SelectNone;
			switchScreen(GameScreen, ShopScreen, false, true);
		}
		break;
	}
}

void Game_Screen::updateDraw() {
	if (Game_Manager::gameOver()) {
		switchScreen(GameScreen, TitleScreen, true);
		return;
	}

	textboxes[0].text.setString(std::format("LVL: {}\nCUR: {}\nNEED: {}",
		Game_Manager::player.getLVL(), Game_Manager::player.getCurEXP(), Game_Manager::player.getLVLUpEXP()));
	textboxes[0].recenterText();

	textboxes[1].text.setString(std::format("HP: {} / {}\nMP: {} / {}",
		Game_Manager::player.getStat(Hp), Game_Manager::player.getStat(Max_Hp),
		Game_Manager::player.getStat(Mp), Game_Manager::player.getStat(Max_Mp)));
	textboxes[1].recenterText();

	texts[9].setString(std::to_string(Game_Manager::player.getFloor()));
	texts[10].setString(std::to_string(Game_Manager::player.getGold()));

	texts[11].setString(Game_Manager::player.getName());

	texts[12].setString(std::format("{} / 50", logs.size()));

	texts[13].setString(std::to_string(Game_Manager::player.getStat(Str)));
	texts[14].setString(std::to_string(Game_Manager::player.getStat(Def)));
	texts[15].setString(std::to_string(Game_Manager::player.getStat(Mgk)));
	texts[16].setString(std::to_string(Game_Manager::player.getStat(Res)));
}

void Game_Screen::changeTheme() {
	rectScan.changeTheme();
	scanText.changeTheme();

	fog.setAreaColor(Setting_Manager::light ? Full_Rectangle::light_bg[theme] : Full_Rectangle::dark_bg[theme]);

	for (Item& item : Game_Manager::items)
		item.changeTheme();
	for (Item& item : Game_Manager::stockItem)
		item.changeTheme();

	for (Spell& spell : Game_Manager::spells)
		spell.changeTheme();
	for (Spell& spell : Game_Manager::stockSpell)
		spell.changeTheme();

	for (Special& special : Game_Manager::stockSpecial)
		special.changeTheme();

	atkIcon.changeTheme();
}

void Game_Screen::changeOpacity() {
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

void Game_Screen::changeRange() {
	window.setView(viewWorld);

	unsigned int x = Game_Manager::player.getPos('x');
	unsigned int y = Game_Manager::player.getPos('y');

	unsigned int range = Game_Manager::plWeapon.getRange();
	float rangeArea = (range * 2 + 1) * TILE;

	unsigned int plX = Game_Manager::player.getPos('x');
	unsigned int plY = Game_Manager::player.getPos('y');

	rangeBox.setPosition(plX - range * TILE, plY - range * TILE);
	rangeBox.setSize(sf::Vector2f(rangeArea, rangeArea));

	window.setView(viewUI);
}

void Game_Screen::changeGrid() {
	window.setView(viewWorld);

	unsigned int x = Game_Manager::player.getPos('x') - DEFAULT_SCREEN_X / 2;
	unsigned int y = Game_Manager::player.getPos('y') - DEFAULT_SCREEN_Y / 2;
	for (unsigned int i = 0; i < 60; i++) {
		i < 30 ? grids[i].setPosition(sf::Vector2f(i * 40 + x, y)) : grids[i].setPosition(sf::Vector2f(x, (i - 30) * 40 + y));
	}

	window.setView(viewUI);
}