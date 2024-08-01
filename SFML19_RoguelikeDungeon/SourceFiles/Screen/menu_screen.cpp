/**
*
* File: menu_screen.cpp
* Description: Contain the implementation of the Menu_Screen class.
*/

#include "Screen/menu_screen.h"
#include "Manager/game_manager.h"

Menu_Screen::Menu_Screen() : Screen(9, 9, false, false) {
	setup_helper(0, NULL,  100.f, 100.f, 630.f, 550.f);
	setup_helper(1, NULL, 1000.f, 310.f, 197.f, 99.f);
	setup_helper(2, NULL, 1000.f, 410.f, 197.f, 99.f);
	setup_helper(3, NULL, 1000.f, 510.f, 197.f, 99.f);
	setup_helper(4, NULL, 1000.f, 610.f, 197.f, 99.f);
	setup_helper(5, NULL, 1000.f, 700.f, 197.f, 99.f);
	setup_helper(6, NULL,  800.f, 700.f, 197.f, 99.f);
	setup_helper(7, NULL,  600.f, 700.f, 197.f, 99.f);
	setup_helper(8, NULL,  400.f, 700.f, 197.f, 99.f);

	setup_helper(0,
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
	setup_helper(1,       "Help", 1010.f, 310.f, NULL, NULL);
	setup_helper(2,        "Map", 1010.f, 410.f, NULL, NULL);
	setup_helper(3,  "Inventory", 1010.f, 510.f, NULL, NULL);
	setup_helper(4,     "Spells", 1010.f, 610.f, NULL, NULL);
	setup_helper(5,       "Save", 1010.f, 710.f, NULL, NULL);
	setup_helper(6,       "Load",  810.f, 710.f, NULL, NULL);
	setup_helper(7,   "Settings",  610.f, 710.f, NULL, NULL);
	setup_helper(8,     "Status", 410.f, 710.f, NULL, NULL);
}

void Menu_Screen::click_event_handler() {
	if (mouse_in_helper(true, 1))
		show_help = !show_help;
	else if (mouse_in_helper(true, 2))
		switch_screen(MenuScreen, MapScreen, false, true);
	else if (mouse_in_helper(true, 3)) {
		Game_Manager::inv_select = Game_Manager::inv_draw_desc = Game_Manager::placeholder_item;
		switch_screen(MenuScreen, InventoryScreen, false, true);
	}
	else if (mouse_in_helper(true, 4)) {
		Game_Manager::spell_select = Game_Manager::spell_desc = Game_Manager::placeholder_spell;
		switch_screen(MenuScreen, SpellScreen, false, true);
	}
	else if (mouse_in_helper(true, 5))
		Game_Manager::save();
	else if (mouse_in_helper(true, 6))
		switch_screen(MenuScreen, LoadScreen, true);
	else if (mouse_in_helper(true, 7))
		switch_screen(MenuScreen, SettingScreen, true);
	else if (mouse_in_helper(true, 8))
		switch_screen(MenuScreen, StatusScreen, false, true);
	else
		return_to_prev_screen(MenuScreen);
}

void Menu_Screen::hover_event_handler() {
	for (unsigned int i = 1; i < 9; i++)
		if (hover_textbox(i, i))
			break;
}

void Menu_Screen::draw() {
	for (unsigned int i = 1; i < rects.size(); i++)
		window.draw(rects[i]);
	for (unsigned int i = 1; i < texts.size(); i++)
		window.draw(texts[i]);

	if (show_help) {
		window.draw(rects[0]);
		window.draw(texts[0]);
	}
}