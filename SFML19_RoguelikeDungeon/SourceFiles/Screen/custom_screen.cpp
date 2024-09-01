/**
*
* File: custom_screen.cpp
* Description: Contain the implementation of the Custom_Screen class.
*/

#include "Screen/custom_screen.h"
#include <Floor/enemy.h>
#include <format>
#include <Manager/database_manager.h>
#include <map>
#include <Screen/screen.h>
#include <Shape/full_rectangle.h>
#include <Shape/full_text.h>
#include <Shape/full_textbox.h>
#include <Shape/full_textinput.h>
#include <string>
#include <Tool/item.h>
#include <Tool/spell.h>
#include <vector>

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
				openMessage(CustomScreen, "Please fill out all fields.", ErrorMsg);
				return;
			}
		}

		if (!updateContent) {
			Database_Manager::executeNonSelectStatement(std::format(
				"INSERT INTO enemies (name, health, health_growth, attack, attack_growth, defense, defense_growth, resistance, resistance_growth, range, experience, experience_growth, attack_type, floor) VALUES('{}', {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {});",
				enemyInputs[0].text.getString().toAnsiString(),
				enemyInputs[1].text.getString().toAnsiString(),
				std::stof(enemyInputs[2].text.getString().toAnsiString()) / 100.f,
				enemyInputs[3].text.getString().toAnsiString(),
				std::stof(enemyInputs[4].text.getString().toAnsiString()) / 100.f,
				enemyInputs[5].text.getString().toAnsiString(),
				std::stof(enemyInputs[6].text.getString().toAnsiString()) / 100.f,
				enemyInputs[7].text.getString().toAnsiString(),
				std::stof(enemyInputs[8].text.getString().toAnsiString()) / 100.f,
				enemyInputs[9].text.getString().toAnsiString(),
				enemyInputs[10].text.getString().toAnsiString(),
				std::stof(enemyInputs[11].text.getString().toAnsiString()) / 100.f,
				enemyInputs[12].text.getString().toAnsiString(),
				enemyInputs[13].text.getString().toAnsiString()
			).c_str());
		}
		else {
			Database_Manager::executeNonSelectStatement(std::format(
				"UPDATE enemies SET name = '{}', health = {}, health_growth = {}, attack = {}, attack_growth = {}, defense = {}, defense_growth = {}, resistance = {}, resistance_growth = {}, range = {}, experience = {}, experience_growth = {}, attack_type = {}, floor = {} WHERE id = {};",
				enemyInputs[0].text.getString().toAnsiString(),
				enemyInputs[1].text.getString().toAnsiString(),
				std::stof(enemyInputs[2].text.getString().toAnsiString()) / 100.f,
				enemyInputs[3].text.getString().toAnsiString(),
				std::stof(enemyInputs[4].text.getString().toAnsiString()) / 100.f,
				enemyInputs[5].text.getString().toAnsiString(),
				std::stof(enemyInputs[6].text.getString().toAnsiString()) / 100.f,
				enemyInputs[7].text.getString().toAnsiString(),
				std::stof(enemyInputs[8].text.getString().toAnsiString()) / 100.f,
				enemyInputs[9].text.getString().toAnsiString(),
				enemyInputs[10].text.getString().toAnsiString(),
				std::stof(enemyInputs[11].text.getString().toAnsiString()) / 100.f,
				enemyInputs[12].text.getString().toAnsiString(),
				enemyInputs[13].text.getString().toAnsiString(),
				updateID
			).c_str());
		}

		for (Full_TextInput& input : enemyInputs)
			input.text.setString("");
		break;
	case ItemMod:
		for (Full_TextInput& input : itemInputs) {
			if (input.text.getString().getSize() == 0) {
				openMessage(CustomScreen, "Please fill out all fields.", ErrorMsg);
				return;
			}
		}

		if (!updateContent) {
			Database_Manager::executeNonSelectStatement(std::format(
				"INSERT INTO items (name, description, abbreviation, buy, sell, range, quantity, type, stat) VALUES('{}', '{}', '{}', {}, {}, {}, {}, {}, {});",
				itemInputs[0].text.getString().toAnsiString(),
				itemInputs[1].text.getString().toAnsiString(),
				itemInputs[2].text.getString().toAnsiString(),
				itemInputs[3].text.getString().toAnsiString(),
				itemInputs[4].text.getString().toAnsiString(),
				itemInputs[5].text.getString().toAnsiString(),
				itemInputs[6].text.getString().toAnsiString(),
				itemInputs[7].text.getString().toAnsiString(),
				itemInputs[8].text.getString().toAnsiString()
			).c_str());
		}
		else {
			Database_Manager::executeNonSelectStatement(std::format(
				"UPDATE items SET name = '{}', description = '{}', abbreviation = '{}', buy = {}, sell = {}, range = {}, quantity = {}, type = {}, stat = {} WHERE id = {};",
				itemInputs[0].text.getString().toAnsiString(),
				itemInputs[1].text.getString().toAnsiString(),
				itemInputs[2].text.getString().toAnsiString(),
				itemInputs[3].text.getString().toAnsiString(),
				itemInputs[4].text.getString().toAnsiString(),
				itemInputs[5].text.getString().toAnsiString(),
				itemInputs[6].text.getString().toAnsiString(),
				itemInputs[7].text.getString().toAnsiString(),
				itemInputs[8].text.getString().toAnsiString(),
				updateID
			).c_str());
		}

		for (Full_TextInput& input : itemInputs)
			input.text.setString("");
		break;
	case SpellMod:
		for (Full_TextInput& input : spellInputs) {
			if (input.text.getString().getSize() == 0) {
				openMessage(CustomScreen, "Please fill out all fields.", ErrorMsg);
				return;
			}
		}

		if (!updateContent) {
			Database_Manager::executeNonSelectStatement(std::format(
				"INSERT INTO spells (name, description, abbreviation, buy, sell, range, quantity, mp, percentage) VALUES('{}', '{}', '{}', {}, {}, {}, {}, {}, {});",
				spellInputs[0].text.getString().toAnsiString(),
				spellInputs[1].text.getString().toAnsiString(),
				spellInputs[2].text.getString().toAnsiString(),
				spellInputs[3].text.getString().toAnsiString(),
				spellInputs[4].text.getString().toAnsiString(),
				spellInputs[5].text.getString().toAnsiString(),
				spellInputs[6].text.getString().toAnsiString(),
				spellInputs[7].text.getString().toAnsiString(),
				std::stof(spellInputs[8].text.getString().toAnsiString()) / 100.f
			).c_str());
		}
		else {
			Database_Manager::executeNonSelectStatement(std::format(
				"UPDATE spells SET name = '{}', description = '{}', abbreviation = '{}', buy = {}, sell = {}, range = {}, quantity = {}, mp = {}, percentage = {} WHERE id = {};",
				spellInputs[0].text.getString().toAnsiString(),
				spellInputs[1].text.getString().toAnsiString(),
				spellInputs[2].text.getString().toAnsiString(),
				spellInputs[3].text.getString().toAnsiString(),
				spellInputs[4].text.getString().toAnsiString(),
				spellInputs[5].text.getString().toAnsiString(),
				spellInputs[6].text.getString().toAnsiString(),
				spellInputs[7].text.getString().toAnsiString(),
				std::stof(spellInputs[8].text.getString().toAnsiString()) / 100.f,
				updateID
			).c_str());
		}

		for (Full_TextInput& input : spellInputs)
			input.text.setString("");
		break;
	}
	openMessage(CustomScreen, "Successfully created entity.\nReset game to see changes.", SuccessMsg);
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
	openMessage(CustomScreen, "Successfully deleted entity.\nReset game to see changes.", SuccessMsg);
	addContent = updateContent = false;
});

unsigned int Custom_Screen::idOffset = 0;
unsigned int Custom_Screen::boxIndex = 0;

std::map<unsigned int, EnemyFull>::iterator Custom_Screen::enemyIter;

std::map<unsigned int, Spell>::iterator Custom_Screen::spellIter;

std::map<unsigned int, Item>::iterator Custom_Screen::itemIter;

Custom_Screen::Custom_Screen() : Screen(true) {
	textboxH("Enemies", 50.f, 20.f, 120.f, 50.f, []() {
		line.setPosition(80.f, 60.f);
		currentMod = EnemyMod;
		addContent = updateContent = false;
		boxIndex = 0;
		getEnemies();
	});
	textboxH("Items", 250.f, 20.f, 100.f, 50.f, []() {
		idOffset = 100;
		line.setPosition(265.f, 60.f);
		currentMod = ItemMod;
		addContent = updateContent = false;
		boxIndex = 0;
		getItems();
	});
	textboxH("Spells", 450.f, 20.f, 100.f, 50.f, []() {
		idOffset = 100;
		line.setPosition(465.f, 60.f);
		currentMod = SpellMod;
		addContent = updateContent = false;
		boxIndex = 0;
		getSpells();
	});

	textboxH("Previous Page", 640.f, 20.f, 200.f, 50.f, []() {
		if (boxIndex >= 20) {
			boxIndex -= 20;

			switch (currentMod) {
				case EnemyMod:
					getEnemies();
					break;
				case ItemMod:
					getItems();
					break;
				case SpellMod:
					getSpells();
					break;
			}
		}
	});
	textboxH("Next Page", 850.f, 20.f, 160.f, 50.f, []() {
		switch (currentMod) {
		case EnemyMod:
			if (boxIndex + 20 < Enemy::enemies.size()) {
				boxIndex += 20;
				getEnemies();
			}
			break;
		case ItemMod:
			if (boxIndex + 20 < Item::items.size()) {
				boxIndex += 20;
				getItems();
			}
			break;
		case SpellMod:
			if (boxIndex + 20 < Spell::spells.size()) {
				boxIndex += 20;
				getSpells();
			}
			break;
		}

	});
	textboxH("+", 1035.f, 20.f, 50.f, 50.f, []() {
		addContent = !addContent;
		updateContent = false;
	}, 28.f);

	for (unsigned int i = 0; i < 20; i++)
		boxes.push_back(Full_Textbox("No data", 50.f + (i % 4 * 285), 150.f + (static_cast<float>(i / 4) * 125), 270.f, 100.f, []() {
		}));

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


	spellTexts.push_back(Full_Text(50.f, 100.f, NULL, "Name"));
	spellInputs.push_back(Full_TextInput("", 20, 50.f, 150.f, 500.f, 50.f, AlphanumericValidation));

	spellTexts.push_back(Full_Text(50.f, 250.f, NULL, "Description"));
	spellInputs.push_back(Full_TextInput("", 40, 50.f, 300.f, 1000.f, 50.f, AlphanumspaceValidation));

	spellTexts.push_back(Full_Text(50.f, 400.f, NULL, "Abbreviation"));
	spellInputs.push_back(Full_TextInput("", 3, 50.f, 450.f, 110.f, 50.f, AlphanumericValidation));

	spellTexts.push_back(Full_Text(300.f, 400.f, NULL, "Buy"));
	spellInputs.push_back(Full_TextInput("", 6, 300.f, 450.f, 110.f, 50.f, NumberValidation));

	spellTexts.push_back(Full_Text(550.f, 400.f, NULL, "Sell"));
	spellInputs.push_back(Full_TextInput("", 6, 550.f, 450.f, 110.f, 50.f, NumberValidation));

	spellTexts.push_back(Full_Text(50.f, 550.f, NULL, "Range"));
	spellInputs.push_back(Full_TextInput("", 1, 50.f, 600.f, 110.f, 50.f, NumberValidation));

	spellTexts.push_back(Full_Text(300.f, 550.f, NULL, "Damage"));
	spellInputs.push_back(Full_TextInput("", 3, 300.f, 600.f, 110.f, 50.f, NumberValidation));

	spellTexts.push_back(Full_Text(550.f, 550.f, NULL, "MP"));
	spellInputs.push_back(Full_TextInput("", 3, 550.f, 600.f, 110.f, 50.f, NumberValidation));

	spellTexts.push_back(Full_Text(700.f, 550.f, NULL, "Damage Growth (%)"));
	spellInputs.push_back(Full_TextInput("", 6, 700.f, 600.f, 110.f, 50.f, NumberValidation));


	itemTexts.push_back(Full_Text(50.f, 100.f, NULL, "Name"));
	itemInputs.push_back(Full_TextInput("", 20, 50.f, 150.f, 500.f, 50.f, AlphanumericValidation));

	itemTexts.push_back(Full_Text(50.f, 250.f, NULL, "Description"));
	itemInputs.push_back(Full_TextInput("", 40, 50.f, 300.f, 1000.f, 50.f, AlphanumspaceValidation));

	itemTexts.push_back(Full_Text(50.f, 400.f, NULL, "Abbreviation"));
	itemInputs.push_back(Full_TextInput("", 3, 50.f, 450.f, 110.f, 50.f, AlphanumericValidation));

	itemTexts.push_back(Full_Text(300.f, 400.f, NULL, "Buy"));
	itemInputs.push_back(Full_TextInput("", 6, 300.f, 450.f, 110.f, 50.f, NumberValidation));

	itemTexts.push_back(Full_Text(550.f, 400.f, NULL, "Sell"));
	itemInputs.push_back(Full_TextInput("", 6, 550.f, 450.f, 110.f, 50.f, NumberValidation));

	itemTexts.push_back(Full_Text(50.f, 550.f, NULL, "Range"));
	itemInputs.push_back(Full_TextInput("", 1, 50.f, 600.f, 110.f, 50.f, NumberValidation));

	itemTexts.push_back(Full_Text(300.f, 550.f, NULL, "Damage/Armor"));
	itemInputs.push_back(Full_TextInput("", 3, 300.f, 600.f, 110.f, 50.f, NumberValidation));

	itemTexts.push_back(Full_Text(550.f, 550.f, NULL, "Weapon (0)/Armor (1)"));
	itemInputs.push_back(Full_TextInput("", 1, 550.f, 600.f, 110.f, 50.f, TrueFalseValidation));

	itemTexts.push_back(Full_Text(700.f, 400.f, NULL, "STR (2), MGK (3), DEF (4), RES (5)"));
	itemInputs.push_back(Full_TextInput("", 1, 700.f, 450.f, 110.f, 50.f, NumberValidation));
}

bool Custom_Screen::handleClickEvent() {
	if (mouseInButton(ExitButton)) {
		switchScreen(display, TitleScreen, false, true);
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

void Custom_Screen::handleHoverEvent() {
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
			for (Full_Text& txt : itemTexts)
				window.draw(txt);
			for (Full_TextInput& input : itemInputs) {
				window.draw(input.rect);
				window.draw(input.text);
			}
			break;
		case SpellMod:
			for (Full_Text& txt : spellTexts)
				window.draw(txt);
			for (Full_TextInput& input : spellInputs) {
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

void Custom_Screen::changeTheme() {
	line.changeTheme();
	create.text.changeTheme();
	create.rect.changeTheme();
	delContent.text.changeTheme();
	delContent.rect.changeTheme();

	for (unsigned int i = 0; i < boxes.size(); i++) {
		boxes[i].text.changeTheme();
		boxes[i].rect.changeTheme();
	}
}

void Custom_Screen::getEnemies() {
	unsigned int i = 0;
	for (enemyIter = std::next(Enemy::enemies.begin(), boxIndex); enemyIter != Enemy::enemies.end() && i < 20; enemyIter++, i++) {
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
			enemyInputs[12].text.setString(std::to_string(en.growth.type));
			enemyInputs[13].text.setString(std::to_string(en.growth.minimumFloor));
			});
		boxes[i].recenterText();
	}

	for (; i < 20; i++) {
		boxes[i].text.setString("No data");
		boxes[i].updateCallback([]() {});
		boxes[i].recenterText();
	}
}

void Custom_Screen::getSpells() {
	unsigned int i = 0;
	for (spellIter = std::next(Spell::spells.begin(), boxIndex); spellIter != Spell::spells.end() && i < 20; spellIter++, i++) {
		unsigned int id = spellIter->second.id;
		Spell* spell = &Spell::spells[id];

		boxes[i].text.setString(std::format("{}. {}\n{}", id, spell->name, spell->type == Offensive ? "Editable" : "Uneditable"));
		spell->type == Offensive ? boxes[i].updateCallback([id]() {
			updateContent = true;
			updateID = id;
			Spell* spell = &Spell::spells[id];
			spellInputs[0].text.setString(spell->name);
			spellInputs[1].text.setString(spell->originalDesc);
			spellInputs[2].text.setString(spell->abbrev);
			spellInputs[3].text.setString(std::to_string(spell->buy));
			spellInputs[4].text.setString(std::to_string(spell->sell));
			spellInputs[5].text.setString(std::to_string(spell->range));
			spellInputs[6].text.setString(std::to_string(spell->quantity));
			spellInputs[7].text.setString(std::to_string(spell->mp));
			spellInputs[8].text.setString(std::to_string(int(spell->percentage * 100)));
			}) : boxes[i].updateCallback([](){});
		boxes[i].recenterText();
	}

	for (; i < 20; i++) {
		boxes[i].text.setString("No data");
		boxes[i].updateCallback([]() {});
		boxes[i].recenterText();
	}
}

void Custom_Screen::getItems() {
	unsigned int i = 0;
	for (itemIter = std::next(Item::items.begin(), boxIndex); itemIter != Item::items.end() && i < 20; itemIter++, i++) {
		unsigned int id = itemIter->second.id;
		Item* item = &Item::items[id];

		boxes[i].text.setString(std::format("{}. {}\n{}", id, item->name, item->type <= Armor ? "Editable" : "Uneditable"));
		item->type <= Armor ? boxes[i].updateCallback([id]() {
			updateContent = true;
			updateID = id;
			Item* item = &Item::items[id];
			itemInputs[0].text.setString(item->name);
			itemInputs[1].text.setString(item->originalDesc);
			itemInputs[2].text.setString(item->abbrev);
			itemInputs[3].text.setString(std::to_string(item->buy));
			itemInputs[4].text.setString(std::to_string(item->sell));
			itemInputs[5].text.setString(std::to_string(item->range));
			itemInputs[6].text.setString(std::to_string(item->quantity));
			itemInputs[7].text.setString(std::to_string(item->type));
			itemInputs[8].text.setString(std::to_string(item->stat));
			}) : boxes[i].updateCallback([]() {});
			boxes[i].recenterText();
	}

	for (; i < 20; i++) {
		boxes[i].text.setString("No data");
		boxes[i].updateCallback([]() {});
		boxes[i].recenterText();
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
