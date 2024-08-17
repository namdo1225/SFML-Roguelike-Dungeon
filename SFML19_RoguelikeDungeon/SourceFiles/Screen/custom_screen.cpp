/**
*
* File: custom_screen.cpp
* Description: Contain the implementation of the Custom_Screen class.
*/

#include "Screen/custom_screen.h"
#include <cstdio>
#include <format>
#include <Manager/database_manager.h>
#include <Screen/screen.h>
#include <Shape/full_rectangle.h>
#include <Shape/full_text.h>
#include <Shape/full_textbox.h>
#include <Shape/full_textinput.h>
#include <vector>

char* Custom_Screen::category;

Full_Rectangle Custom_Screen::line = Full_Rectangle(80.f, 60.f, 60.f, 0.5f, false);

Mod Custom_Screen::currentMod = EnemyMod;

bool Custom_Screen::addContent = false;

std::vector<Full_Textbox> Custom_Screen::boxes;

std::vector<Full_Text> Custom_Screen::enemyTexts;

std::vector<Full_Text> Custom_Screen::itemTexts;

std::vector<Full_Text> Custom_Screen::spellTexts;

std::vector<Full_TextInput> Custom_Screen::enemyInputs;

std::vector<Full_TextInput> Custom_Screen::itemInputs;

std::vector<Full_TextInput> Custom_Screen::spellInputs;

Full_Textbox Custom_Screen::create = Full_Textbox("Create", 800.f, 700.f, 100.f, 50.f, []() {
});

unsigned int Custom_Screen::offset = 0;

Custom_Screen::Custom_Screen() : Screen(true) {
	setupTextbox("Enemies", 50.f, 20.f, 120.f, 50.f, []() {
		offset = 100;
		line.setPosition(80.f, 60.f);
		currentMod = EnemyMod;
		addContent = false;
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
		addContent = false;
	});
	setupTextbox("Spells", 450.f, 20.f, 100.f, 50.f, []() {
		offset = 100;
		line.setPosition(465.f, 60.f);
		currentMod = SpellMod;
		addContent = false;
		});

	setupTextbox("Previous Page", 640.f, 20.f, 200.f, 50.f, []() {
	});
	setupTextbox("Next Page", 850.f, 20.f, 160.f, 50.f, []() {
	});
	setupTextbox("+", 1035.f, 20.f, 50.f, 50.f, []() {
		addContent = !addContent;
	}, 28.f);

	for (unsigned int i = 0; i < 20; i++) {
		boxes.push_back(Full_Textbox("No data", 50.f + (i % 4 * 250), 150.f + (static_cast<float>(i / 4) * 125), 200.f, 100.f, []() {
		}));
	}

	enemyTexts.push_back(Full_Text(50.f, 100.f, NULL, "Name"));
	enemyInputs.push_back(Full_TextInput("", 20, 50.f, 150.f, 300.f, 50.f, AlphanumericValidation));

	enemyTexts.push_back(Full_Text(400.f, 100.f, NULL, "Attack Type"));
	enemyInputs.push_back(Full_TextInput("", 1, 400.f, 150.f, 30.f, 50.f, TrueFalseValidation));

	enemyTexts.push_back(Full_Text(50.f, 250.f, NULL, "Health"));
	enemyInputs.push_back(Full_TextInput("", 6, 50.f, 300.f, 110.f, 50.f, NumberValidation));

	enemyTexts.push_back(Full_Text(250.f, 250.f, NULL, "Health Growth (%)"));
	enemyInputs.push_back(Full_TextInput("", 6, 250.f, 300.f, 110.f, 50.f, NumberValidation));

	enemyTexts.push_back(Full_Text(550.f, 250.f, NULL, "Attack"));
	enemyInputs.push_back(Full_TextInput("", 6, 550.f, 300.f, 110.f, 50.f, NumberValidation));

	enemyTexts.push_back(Full_Text(750.f, 250.f, NULL, "Attack Growth (%)"));
	enemyInputs.push_back(Full_TextInput("", 6, 750.f, 300.f, 110.f, 50.f, NumberValidation));


	enemyTexts.push_back(Full_Text(50.f, 400.f, NULL, "Defense"));
	enemyInputs.push_back(Full_TextInput("", 6, 50.f, 450.f, 110.f, 50.f, NumberValidation));

	enemyTexts.push_back(Full_Text(250.f, 400.f, NULL, "Defense Growth (%)"));
	enemyInputs.push_back(Full_TextInput("", 6, 250.f, 450.f, 110.f, 50.f, NumberValidation));

	enemyTexts.push_back(Full_Text(550.f, 400.f, NULL, "Resistance"));
	enemyInputs.push_back(Full_TextInput("", 6, 550.f, 450.f, 110.f, 50.f, NumberValidation));

	enemyTexts.push_back(Full_Text(750.f, 400.f, NULL, "Resistance Growth (%)"));
	enemyInputs.push_back(Full_TextInput("", 6, 750.f, 450.f, 110.f, 50.f, NumberValidation));


	enemyTexts.push_back(Full_Text(50.f, 550.f, NULL, "Range"));
	enemyInputs.push_back(Full_TextInput("", 1, 50.f, 600.f, 110.f, 50.f, NumberValidation));

	enemyTexts.push_back(Full_Text(250.f, 550.f, NULL, "Min Floor to Spawn"));
	enemyInputs.push_back(Full_TextInput("", 6, 250.f, 600.f, 110.f, 50.f, NumberValidation));

	enemyTexts.push_back(Full_Text(550.f, 550.f, NULL, "Experience"));
	enemyInputs.push_back(Full_TextInput("", 6, 550.f, 600.f, 110.f, 50.f, NumberValidation));

	enemyTexts.push_back(Full_Text(750.f, 550.f, NULL, "Experience Growth (%)"));
	enemyInputs.push_back(Full_TextInput("", 6, 750.f, 600.f, 110.f, 50.f, NumberValidation));
}

void Custom_Screen::click_event_handler() {
	if (mouse_in_button(ExitButton))
		switch_screen(display, TitleScreen, false, true);
	if (addContent) {
		switch (currentMod) {
		case EnemyMod:
			for (Full_TextInput& input : enemyInputs)
				input.click();
			break;
		case ItemMod:
			for (Full_TextInput& input : itemInputs)
				input.click();
			break;
		case SpellMod:
			for (Full_TextInput& input : spellInputs)
				input.click();
			break;
		}
	}
}

void Custom_Screen::hover_event_handler() {
	for (Full_Textbox& textbox : boxes)
		textbox.hover();
	create.hover();
}

void Custom_Screen::draw() {
	Screen::draw();
	window.draw(line);

	if (addContent) {
		window.draw(create.rect);
		window.draw(create.text);

		switch (currentMod) {
		case EnemyMod:
			for (Full_Text txt : enemyTexts)
				window.draw(txt);
			for (Full_TextInput input : enemyInputs) {
				window.draw(input.rect);
				window.draw(input.text);
			}
			break;
		case ItemMod:
			for (Full_Text txt : itemTexts)
				window.draw(txt);
			for (Full_TextInput input : itemInputs) {
				window.draw(input.rect);
				window.draw(input.text);
			}
			break;
		case SpellMod:
			for (Full_Text txt : spellTexts)
				window.draw(txt);
			for (Full_TextInput input : spellInputs) {
				window.draw(input.rect);
				window.draw(input.text);
			}
			break;
		}
	}
	else {
		for (Full_Textbox textbox : boxes) {
			window.draw(textbox.rect);
			window.draw(textbox.text);
		}
	}
}

void Custom_Screen::change_theme() {
	line.flip_theme();
	create.text.flip_theme();
	create.rect.flip_theme();

	for (unsigned int i = 0; i < boxes.size(); i++) {
		boxes[i].text.flip_theme();
		boxes[i].rect.flip_theme();
	}
}

void Custom_Screen::executeSQL()
{
}

void Custom_Screen::handleTextEvent() {
	if (addContent) {
		switch (currentMod) {
		case EnemyMod:
			for (Full_TextInput& input : enemyInputs)
				input.handleTextEvent();
			break;
		case ItemMod:
			for (Full_TextInput& input : itemInputs)
				input.handleTextEvent();
			break;
		case SpellMod:
			for (Full_TextInput& input : spellInputs)
				input.handleTextEvent();
			break;
		}
	}
}
