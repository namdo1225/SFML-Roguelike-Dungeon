#include "screen.h"
#include <Floor/enemy.h>
#include <map>
#include <Shape/full_rectangle.h>
#include <Shape/full_text.h>
#include <Shape/full_textbox.h>
#include <Shape/full_textinput.h>
#include <Tool/item.h>
#include <Tool/spell.h>
#include <vector>

#ifndef CUSTOM_SCREEN_H
#define CUSTOM_SCREEN_H

enum Mod { EnemyMod, ItemMod, SpellMod };

/**
* A screen for players to add custom assets.
*/
class Custom_Screen : public Screen {
private:
	static char* category;

	static std::map<unsigned int, EnemyFull>::iterator enemyIter;

	static std::map<unsigned int, Spell>::iterator spellIter;

	static std::map<unsigned int, Item>::iterator itemIter;

	static Full_Rectangle line;

	static std::vector<Full_Textbox> boxes;

	static std::vector<Full_Text> enemyTexts;

	static std::vector<Full_Text> itemTexts;

	static std::vector<Full_Text> spellTexts;

	static std::vector<Full_TextInput> enemyInputs;

	static std::vector<Full_TextInput> itemInputs;

	static std::vector<Full_TextInput> spellInputs;

	static Full_Textbox create;
	static Full_Textbox delContent;

	static Mod currentMod;

	static bool addContent;
	static bool updateContent;
	static unsigned int updateID;

	static unsigned int idOffset;
	static unsigned int boxIndex;

	/**
	* Get enemies and display them in boxes.
	*/
	static void getEnemies();

	/**
	* Get spells and display them in boxes.
	*/
	static void getSpells();

	/**
	* Get items and display them in boxes.
	*/
	static void getItems();

public:
	Custom_Screen();

	bool handleClickEvent();

	void handleHoverEvent();

	void draw();

	void changeTheme();

	void handleTextEvent();
};

#endif