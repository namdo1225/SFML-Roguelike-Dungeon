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
	setup_helper(NULL,  100.f, 100.f, 630.f, 550.f);
	setup_helper(
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

	setupTextbox("Help", 1000.f, 300.f, 197.f, 99.f, []() {
		help = !help;
	});
	setupTextbox( "Map", 1000.f, 400.f, 197.f, 99.f, []() {
		switch_screen(MenuScreen, MapScreen, false, true);
	});
	setupTextbox("Inventory", 1000.f, 500.f, 197.f, 99.f, []() {
		Game_Manager::inv_select = Game_Manager::inv_draw_desc = Game_Manager::placeholder_item;
		switch_screen(MenuScreen, InventoryScreen, false, true);
	});
	setupTextbox("Spells", 1000.f, 600.f, 197.f, 99.f, []() {
		Game_Manager::spell_select = Game_Manager::spell_desc = Game_Manager::placeholder_spell;
		switch_screen(MenuScreen, SpellScreen, false, true);
	});
	setupTextbox("Load", 800.f, 700.f, 197.f, 99.f, []() {
		switch_screen(MenuScreen, LoadScreen, true);
	});
	setupTextbox("Settings", 600.f, 700.f, 197.f, 99.f, []() {
		switch_screen(MenuScreen, SettingScreen, true);
	});
	setupTextbox("Status", 400.f, 700.f, 197.f, 99.f, []() {
		switch_screen(MenuScreen, StatusScreen, true);;
	});
	setupTextbox("Level", 200.f, 700.f, 197.f, 99.f, []() {
		map_txts["stat_left"].setString(std::to_string(Game_Manager::player.get_pts()));
		switch_screen(MenuScreen, LevelScreen, false, true);
	});
	setupTextbox("Save", 0.f, 700.f, 197.f, 99.f, []() {
		Game_Manager::save();
	});
}

void Menu_Screen::click_event_handler() {
	if (mouse_in_button(ExitButton))
		return_to_prev_screen(MenuScreen);
}

void Menu_Screen::draw() {
	for (unsigned int i = 0; i < textboxes.size(); i++) {
		window.draw(textboxes[i].rect);
		window.draw(textboxes[i].text);
	}

	if (exit_button) {
		window.draw(map_rects["exit"]);
		window.draw(map_txts["exit"]);
	}

	if (help) {
		window.draw(rects[0]);
		window.draw(texts[0]);
	}
}