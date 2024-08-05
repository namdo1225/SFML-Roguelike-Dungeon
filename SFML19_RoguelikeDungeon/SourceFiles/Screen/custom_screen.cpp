/**
*
* File: custom_screen.cpp
* Description: Contain the implementation of the Custom_Screen class.
*/

#include "Screen/custom_screen.h"
#include <cstdio>
#include <Manager/database_manager.h>
#include <Manager/font_manager.h>
#include <Screen/screen.h>
#include <Shape/full_rectangle.h>
#include <Shape/full_textbox.h>
#include <vector>
#include <format>

char* Custom_Screen::category;

Full_Rectangle Custom_Screen::line = Full_Rectangle(80.f, 60.f, 60.f, 0.5f, false);

Mod Custom_Screen::currentMod = EnemyMod;

bool Custom_Screen::addContent = false;

std::vector<Full_Textbox> Custom_Screen::boxes;

unsigned int Custom_Screen::offset = 0;

Custom_Screen::Custom_Screen() : Screen(true) {
	setupTextbox("Enemies", 50.f, 20.f, 120.f, 50.f, []() {
		offset = 100;
		line.setPosition(80.f, 60.f);
		currentMod = EnemyMod;
		Database_Manager::executeSelect(std::format("SELECT * FROM enemies WHERE id >= 100 LIMIT 20 OFFSET {};", offset).c_str(), [](void* data, int argc, char** argv, char** azColName) {
			int i;

			for (i = 0; i < argc; i++) {
				printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
			}

			printf("\n");

			return 0;
			}
		);
	});
	setupTextbox("Items", 250.f, 20.f, 100.f, 50.f, []() {
		offset = 100;
		line.setPosition(265.f, 60.f);
		currentMod = ItemMod;
	});
	setupTextbox("Spells", 450.f, 20.f, 100.f, 50.f, []() {
		offset = 100;
		line.setPosition(465.f, 60.f);
		currentMod = SpellMod;
		});

	setupTextbox("Previous Page", 640.f, 20.f, 200.f, 50.f, []() {
	});
	setupTextbox("Next Page", 850.f, 20.f, 160.f, 50.f, []() {
	});
	setupTextbox("+", 1035.f, 20.f, 50.f, 50.f, []() {
	}, 28.f);

	for (unsigned int i = 0; i < 20; i++) {
		boxes.push_back(Full_Textbox("No data", 50.f + (i % 4 * 250), 150.f + (static_cast<float>(i / 4) * 125), 200.f, 100.f, []() {
			}));
	}
}

void Custom_Screen::click_event_handler() {
	if (mouse_in_button(ExitButton))
		switch_screen(display, TitleScreen, false, true);
}

void Custom_Screen::hover_event_handler() {
	for (Full_Textbox& textbox : boxes)
		textbox.hover();
}

void Custom_Screen::draw() {
	Screen::draw();
	window.draw(line);
	for (Full_Textbox textbox : boxes) {
		window.draw(textbox.rect);
		window.draw(textbox.text);
	}
}

void Custom_Screen::change_theme() {
	line.flip_theme();
	for (unsigned int i = 0; i < boxes.size(); i++) {
		boxes[i].text.flip_theme();
		boxes[i].text.setFont(Font_Manager::get_selected());
		boxes[i].rect.flip_theme();
	}
}

void Custom_Screen::executeSQL()
{
}
