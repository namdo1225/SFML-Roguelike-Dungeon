/**
*
* File: screen.h
* Description: Contain the declaration of the Screen class, which represents a game scene.
*/

#ifndef SCREEN_H
#define SCREEN_H

#include "..\SFML-2.5.1/include/SFML/Graphics.hpp"
#include <iostream>
#include "Shape/full_text.h"
#include "Shape/full_rectangle.h"
#include "Manager/setting_manager.h"
#include "Manager/sf_manager.h"

enum Display { TitleScreen, NameScreen, StatScreen, LevelScreen,
	GameScreen, MenuScreen, InventoryScreen, SpellScreen, MapScreen, ShopScreen, ScanScreen, HelpScreen,
	LoadScreen, SpellAttackScreen, LogScreen, SettingScreen, ExitScreen
};

enum ReusableButton { ExitButton, ConfirmButton, StatFulLButton, StatIncreaseButton, UseButton, DiscardButton,
ClearButton, };

class Screen : public SF_Manager {
protected:
	static bool loaded;
	bool exit_button = false;
	bool confirm_button = false;
	bool show_bg = true;
	bool text_handler_enabled = false;
	bool update = false;

	std::vector<Full_Text> texts;
	std::vector<Full_Rectangle> rects;
	static Full_Rectangle background;

	// Stat selection common UI:
	const static char* FULL_STATS[];

	static Full_Text stat_full_txts[NUM_NON_CUR_STATS];
	static Full_Text stat_curr_txts[NUM_NON_CUR_STATS];
	static Full_Text stat_curr_arrows[NUM_NON_CUR_STATS * 2];
	static Full_Text stat_guide_txt;
	static Full_Text stat_left_txt;
	static Full_Text stat_left_pts;

	// Inventory/spell selection common UI:

	static Full_Rectangle inv_sp_slots[MAX_INV_SPELL_SLOTS];

	static Full_Rectangle inv_sp_use_rect;
	static Full_Text inv_sp_use_txt;

	static Full_Rectangle inv_sp_desc_rect;
	static Full_Text inv_sp_desc_txt;

	static Full_Rectangle inv_sp_discard_rect;
	static Full_Text inv_sp_discard_txt;

	static Full_Rectangle inv_sp_cur_slot;

	static Full_Text inv_sp_gold_txt;
	static Full_Text inv_sp_gold_amount_txt;

	// Name selection, save, and load common UI:

	static Full_Rectangle clear_rect;
	static Full_Text clear_txt;

	static Full_Rectangle name_rect;
	static Full_Text name_txt;

	static Full_Text confirm_txt;
	static Full_Rectangle confirm_rect;

	void setup_helper(bool element, unsigned int i, const char* text, float x, float y, float sx, float sy,
		unsigned int theme = 0, bool hoverable = true, bool is_light = false, bool override_theme = false);

	bool mouse_in_helper(bool element, unsigned int i);

	bool mouse_in_button(ReusableButton button);

	bool mouse_in_slot(unsigned int i);

	void hover_button(ReusableButton button);

	void hover_slot(unsigned int i);

	static void return_to_prev_screen(Display screen);

	static void switch_screen(Display old_screen, Display new_screen, bool push, bool clear = false);

	bool hover_textbox(unsigned int i, int j);

	bool hover_helper(bool element, unsigned int i);

	virtual void change_theme();

public:
	static Display display;
	static std::vector<Display> prev_displays;

	const static unsigned int num_screens = 17;
	static bool visibilities[num_screens];
	static std::unique_ptr<Screen> screens[num_screens];

	/**
	* Constructor for Screen.
	*/
	Screen(unsigned int txt, unsigned int rect, bool exit_button = false, bool show_bg = true, bool confirm_button = false,
		bool text_handler_enabled = false);

	/**
	* Draw the screen.
	*/
	virtual void draw();

	/**
	* Updates dynamic texts for a screen.
	*/
	virtual void update_draw();

	/**
	* Mouse event handler for the screen.
	*/
	virtual void mouse_event_handler();

	/**
	* Click event handler for the screen.
	*/
	virtual void click_event_handler() = 0;

	/**
	* Hover event handler for the screen.
	*/
	virtual void hover_event_handler() = 0;

	/**
	* Key press event handler for the screen.
	*/
	virtual void key_event_handler();

	/**
	* Text entered event handler for the screen.
	*/
	virtual void text_event_handler();

	static void text_event_helper();

	static void setup();

	static void show_dialog(Display old_screen, Display new_screen);

	static void change_settings();

	bool get_update();

private:
	static Full_Text exit_txt;
	static Full_Rectangle exit_rect;
};


#endif