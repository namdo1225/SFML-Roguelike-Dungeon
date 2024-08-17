/**
*
* File: custom_screen.h
* Description: Contain the declaration of the Custom_Screen class, which represents a screen for players to add custom assets.
*/

#include "screen.h"
#include <Shape/full_rectangle.h>
#include <Shape/full_text.h>
#include <Shape/full_textbox.h>
#include <Shape/full_textinput.h>
#include <vector>

#ifndef CUSTOM_SCREEN_H
#define CUSTOM_SCREEN_H

enum Mod { EnemyMod, ItemMod, SpellMod };

class Custom_Screen : public Screen {
private:
	static char* category;

	static Full_Rectangle line;

	static std::vector<Full_Textbox> boxes;

	static std::vector<Full_Text> enemyTexts;

	static std::vector<Full_Text> itemTexts;

	static std::vector<Full_Text> spellTexts;

	static std::vector<Full_TextInput> enemyInputs;

	static std::vector<Full_TextInput> itemInputs;

	static std::vector<Full_TextInput> spellInputs;

	static Full_Textbox create;

	static Mod currentMod;

	static bool addContent;

	static unsigned int offset;

public:
	Custom_Screen();

	void click_event_handler();

	void hover_event_handler();

	void draw();

	void change_theme();

	void executeSQL();

	void handleTextEvent();
};

#endif