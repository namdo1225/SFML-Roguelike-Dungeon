/**
*
* File: menu_screen.cpp
* Description: Contain the implementation of the Menu_Screen class.
*/

#include "Manager/game_manager.h"
#include "Screen/menu_screen.h"
#include <Screen/screen.h>
#include <string>

bool Menu_Screen::help = false;

Menu_Screen::Menu_Screen() : Screen(true, false) {
	textRectH(NULL,  100.f, 100.f, 630.f, 550.f);
	textRectH(
		"Arrow keys: Move player."
		"\nW: Wait. Skip turn."
		"\nQ: Move up a floor if you are standing on a stair."
		"\nS: Go to the shop if you are standing on a shop."
		"\nESC: Bring up exit menu."
		"\nG: Enable/Disable grid."
		"\nO: Change opacity."
		"\nR: Show your weapon's range."
		"\n\nClick on an enemy to attack them."
		"\nThe slots let you quick access your items."
		"\nClick on the center to use the item/spell."
		"\nClick on 'S' button and hover on enemies to scan\ntheir stat.",
		110.f, 110.f, NULL, NULL);

	textboxH("Help", 1000.f, 700.f, 197.f, 99.f, []() {
		help = !help;
	});
	textboxH( "Map", 1000.f, 400.f, 197.f, 99.f, []() {
		switchScreen(MenuScreen, MapScreen, false, true);
	});
	textboxH("Inventory", 1000.f, 500.f, 197.f, 99.f, []() {
		Game_Manager::selectedInv = SelectNone;
		switchScreen(MenuScreen, InventoryScreen, false, true);
	});
	textboxH("Spells", 1000.f, 600.f, 197.f, 99.f, []() {
		Game_Manager::selectedSpell = SelectNone;
		switchScreen(MenuScreen, SpellScreen, false, true);
	});
	textboxH("Load", 800.f, 700.f, 197.f, 99.f, []() {
		switchScreen(MenuScreen, LoadScreen, true);
	});
	textboxH("Settings", 600.f, 700.f, 197.f, 99.f, []() {
		switchScreen(MenuScreen, SettingScreen, true);
	});
	textboxH("Status", 400.f, 700.f, 197.f, 99.f, []() {
		switchScreen(MenuScreen, StatusScreen, true);
	});
	textboxH("Level", 200.f, 700.f, 197.f, 99.f, []() {
		map_txts["stat_left"].setString(std::to_string(Game_Manager::player.getStatPts()));
		switchScreen(MenuScreen, LevelScreen, false, true);
	});
	textboxH("Save", 0.f, 700.f, 197.f, 99.f, []() {
		Game_Manager::save();
	});
}

bool Menu_Screen::handleClickEvent() {
	if (mouseInButton(ExitButton)) {
		goToPrevScreen(MenuScreen);
		return true;
	}
}

void Menu_Screen::draw() {
	for (unsigned int i = 0; i < textboxes.size(); i++) {
		window.draw(textboxes[i].rect);
		window.draw(textboxes[i].text);
	}

	window.draw(map_rects["exit"]);
	window.draw(map_txts["exit"]);

	if (help) {
		window.draw(rects[0]);
		window.draw(texts[0]);
	}
}