/**
*
* File: custom_screen.cpp
* Description: Contain the implementation of the Custom_Screen class.
*/

#include "Screen/custom_screen.h"
#include <format>
#include <Manager/database_manager.h>
#include <map>
#include <Screen/screen.h>
#include <Shape/full_rectangle.h>
#include <Shape/full_text.h>
#include <Shape/full_textbox.h>
#include <Shape/full_textinput.h>
#include <string>
#include <vector>
#include <Floor/enemy.h>

char* Custom_Screen::category;

Full_Rectangle Custom_Screen::line = Full_Rectangle(80.f, 60.f, 60.f, 0.5f, false);

Mod Custom_Screen::currentMod = EnemyMod;

bool Custom_Screen::addContent = false;
bool Custom_Screen::updateContent = false;

unsigned int Custom_Screen::updateID = 0;

std::vector<Full_Textbox> Custom_Screen::boxes;

std::vector<Full_TextInput> Custom_Screen::enemyInputs;

std::vector<Full_TextInput> Custom_Screen::itemInputs;

std::vector<Full_TextInput> Custom_Screen::spellInputs;

std::vector<Full_Text> Custom_Screen::enemyTexts;

std::vector<Full_Text> Custom_Screen::itemTexts;

std::vector<Full_Text> Custom_Screen::spellTexts;

Full_Textbox Custom_Screen::create = Full_Textbox("Finish", 800.f, 700.f, 100.f, 50.f, []() {
	switch (currentMod) {
	case EnemyMod:
		for (Full_TextInput& input : enemyInputs) {
			if (input.text.getString().getSize() == 0) {
				showMessage(CustomScreen, "Please fill out all fields.", ErrorMsg);
				return;
			}
		}

		if (!updateContent) {
			Database_Manager::executeNonSelectStatement(std::format(
				"INSERT INTO enemies (name, health, health_growth, attack, attack_growth, defense, defense_growth, resistance, resistance_growth, range, experience, experience_growth, attack_type, floor) VALUES('{}', {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {});",
				enemyInputs[0].text.getString().toAnsiString().c_str(),
				enemyInputs[1].text.getString().toAnsiString().c_str(),
				std::stof(enemyInputs[2].text.getString().toAnsiString().c_str()) / 100.f,
				enemyInputs[3].text.getString().toAnsiString().c_str(),
				std::stof(enemyInputs[4].text.getString().toAnsiString().c_str()) / 100.f,
				enemyInputs[5].text.getString().toAnsiString().c_str(),
				std::stof(enemyInputs[6].text.getString().toAnsiString().c_str()) / 100.f,
				enemyInputs[7].text.getString().toAnsiString().c_str(),
				std::stof(enemyInputs[8].text.getString().toAnsiString().c_str()) / 100.f,
				enemyInputs[9].text.getString().toAnsiString().c_str(),
				enemyInputs[10].text.getString().toAnsiString().c_str(),
				std::stof(enemyInputs[11].text.getString().toAnsiString().c_str()) / 100.f,
				enemyInputs[12].text.getString().toAnsiString().c_str(),
				enemyInputs[13].text.getString().toAnsiString().c_str()
			).c_str());
		}
		else {
			Database_Manager::executeNonSelectStatement(std::format(
				"UPDATE enemies SET name = '{}', health = {}, health_growth = {}, attack = {}, attack_growth = {}, defense = {}, defense_growth = {}, resistance = {}, resistance_growth = {}, range = {}, experience = {}, experience_growth = {}, attack_type = {}, floor = {} WHERE id = {};",
				enemyInputs[0].text.getString().toAnsiString().c_str(),
				enemyInputs[1].text.getString().toAnsiString().c_str(),
				std::stof(enemyInputs[2].text.getString().toAnsiString().c_str()) / 100.f,
				enemyInputs[3].text.getString().toAnsiString().c_str(),
				std::stof(enemyInputs[4].text.getString().toAnsiString().c_str()) / 100.f,
				enemyInputs[5].text.getString().toAnsiString().c_str(),
				std::stof(enemyInputs[6].text.getString().toAnsiString().c_str()) / 100.f,
				enemyInputs[7].text.getString().toAnsiString().c_str(),
				std::stof(enemyInputs[8].text.getString().toAnsiString().c_str()) / 100.f,
				enemyInputs[9].text.getString().toAnsiString().c_str(),
				enemyInputs[10].text.getString().toAnsiString().c_str(),
				std::stof(enemyInputs[11].text.getString().toAnsiString().c_str()) / 100.f,
				enemyInputs[12].text.getString().toAnsiString().c_str(),
				enemyInputs[13].text.getString().toAnsiString().c_str(),
				updateID
			).c_str());
		}

		for (Full_TextInput& input : enemyInputs)
			input.text.setString("");
		break;
	case ItemMod:
		break;
	case SpellMod:
		break;
	}
	showMessage(CustomScreen, "Successfully created entity.\nReset game to see changes.", SuccessMsg);
	addContent = updateContent = false;
});

Full_Textbox Custom_Screen::delContent = Full_Textbox("Delete", 1000.f, 700.f, 100.f, 50.f, []() {
	std::string delTable = "enemies";
	switch (currentMod) {
	case ItemMod:
		delTable = "items";
		break;
	case SpellMod:
		delTable = "spells";
		break;
	}
	Database_Manager::executeNonSelectStatement(std::format("DELETE FROM {} WHERE id = {}", delTable, updateID).c_str());
	showMessage(CustomScreen, "Successfully deleted entity.\nReset game to see changes.", SuccessMsg);
	addContent = updateContent = false;
});

unsigned int Custom_Screen::idOffset = 0;
unsigned int Custom_Screen::boxIndex = 0;

std::map<unsigned int, EnemyFull>::iterator Custom_Screen::enemyIter;

Custom_Screen::Custom_Screen() : Screen(true) {
	setupTextbox("Enemies", 50.f, 20.f, 120.f, 50.f, []() {
		line.setPosition(80.f, 60.f);
		currentMod = EnemyMod;
		addContent = updateContent = false;
		boxIndex = 0;

		unsigned int i = 0;
		for (enemyIter = Enemy::enemies.begin(); enemyIter != Enemy::enemies.end() && i < 20; ++enemyIter) {
			unsigned int id = enemyIter->second.growth.id;
			boxes[i].text.setString(std::format("{}. {}\nMin floor: {}", enemyIter->second.growth.id, enemyIter->second.growth.name, enemyIter->second.growth.minimumFloor).c_str());
			boxes[i].updateCallback([id]() {
				updateContent = true;
				updateID = id;
				EnemyFull& en = Enemy::enemies[id];
				enemyInputs[0].text.setString(en.growth.name);
				enemyInputs[1].text.setString(std::to_string(en.stat.hp));
				enemyInputs[2].text.setString(std::to_string(int(en.growth.hpGrowth * 100)));
				enemyInputs[3].text.setString(std::to_string(en.stat.atk));
				enemyInputs[4].text.setString(std::to_string(int(en.growth.atkGrowth * 100)));
				enemyInputs[5].text.setString(std::to_string(en.stat.def));
				enemyInputs[6].text.setString(std::to_string(int(en.growth.defGrowth * 100)));
				enemyInputs[7].text.setString(std::to_string(en.stat.res));
				enemyInputs[8].text.setString(std::to_string(int(en.growth.resGrowth * 100)));
				enemyInputs[9].text.setString(std::to_string(en.stat.range));
				enemyInputs[10].text.setString(std::to_string(en.stat.exp));
				enemyInputs[11].text.setString(std::to_string(int(en.growth.expGrowth * 100)));
				enemyInputs[12].text.setString(std::to_string(en.stat.type));
				enemyInputs[13].text.setString(std::to_string(en.growth.minimumFloor));
			});
			boxes[i].recenterText();
			i++;
		}
	});
	setupTextbox("Items", 250.f, 20.f, 100.f, 50.f, []() {
		idOffset = 100;
		line.setPosition(265.f, 60.f);
		currentMod = ItemMod;
		addContent = updateContent = false;
		boxIndex = 0;
	});
	setupTextbox("Spells", 450.f, 20.f, 100.f, 50.f, []() {
		idOffset = 100;
		line.setPosition(465.f, 60.f);
		currentMod = SpellMod;
		addContent = updateContent = false;
		boxIndex = 0;
		});

	setupTextbox("Previous Page", 640.f, 20.f, 200.f, 50.f, []() {
	});
	setupTextbox("Next Page", 850.f, 20.f, 160.f, 50.f, []() {
	});
	setupTextbox("+", 1035.f, 20.f, 50.f, 50.f, []() {
		addContent = !addContent;
		updateContent = false;
	}, 28.f);

	for (unsigned int i = 0; i < 20; i++) {
		boxes.push_back(Full_Textbox("No data", 50.f + (i % 4 * 250), 150.f + (static_cast<float>(i / 4) * 125), 200.f, 100.f, []() {
		}));
	}

	enemyTexts.push_back(Full_Text(50.f, 100.f, NULL, "Name"));
	enemyInputs.push_back(Full_TextInput("", 20, 50.f, 150.f, 300.f, 50.f, AlphanumericValidation));

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

	enemyTexts.push_back(Full_Text(550.f, 550.f, NULL, "Experience"));
	enemyInputs.push_back(Full_TextInput("", 6, 550.f, 600.f, 110.f, 50.f, NumberValidation));

	enemyTexts.push_back(Full_Text(750.f, 550.f, NULL, "Experience Growth (%)"));
	enemyInputs.push_back(Full_TextInput("", 6, 750.f, 600.f, 110.f, 50.f, NumberValidation));

	enemyTexts.push_back(Full_Text(400.f, 100.f, NULL, "Attack Type"));
	enemyInputs.push_back(Full_TextInput("", 1, 400.f, 150.f, 30.f, 50.f, TrueFalseValidation));

	enemyTexts.push_back(Full_Text(250.f, 550.f, NULL, "Min Floor to Spawn"));
	enemyInputs.push_back(Full_TextInput("", 6, 250.f, 600.f, 110.f, 50.f, NumberValidation));
}

bool Custom_Screen::click_event_handler() {
	if (mouse_in_button(ExitButton)) {
		switch_screen(display, TitleScreen, false, true);
		return true;
	}
	if (addContent || updateContent) {
		switch (currentMod) {
		case EnemyMod:
			for (Full_TextInput& input : enemyInputs)
				if (input.click())
					return true;
			break;
		case ItemMod:
			for (Full_TextInput& input : itemInputs)
				if (input.click())
					return true;
			break;
		case SpellMod:
			for (Full_TextInput& input : spellInputs)
				if (input.click())
					return true;
			break;
		}

		if (create.click() || (updateContent && delContent.click()))
			return true;
	}
	else
		for (Full_Textbox& box : boxes)
			if (box.click())
				return true;

	return false;
}

void Custom_Screen::hover_event_handler() {
	for (Full_Textbox& textbox : boxes)
		textbox.hover();
	create.hover();
	delContent.hover();
}

void Custom_Screen::draw() {
	Screen::draw();
	window.draw(line);

	if (addContent || updateContent) {
		window.draw(create.rect);
		window.draw(create.text);

		if (updateContent) {
			window.draw(delContent.rect);
			window.draw(delContent.text);
		}

		switch (currentMod) {
		case EnemyMod:
			for (Full_Text& txt : enemyTexts)
				window.draw(txt);
			for (Full_TextInput& input : enemyInputs) {
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
	delContent.text.flip_theme();
	delContent.rect.flip_theme();

	for (unsigned int i = 0; i < boxes.size(); i++) {
		boxes[i].text.flip_theme();
		boxes[i].rect.flip_theme();
	}
}

void Custom_Screen::handleTextEvent() {
	if (addContent || updateContent) {
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
