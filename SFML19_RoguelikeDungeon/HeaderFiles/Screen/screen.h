/**
*
* File: screen.h
* Description: Contain the declaration of the Screen class, which represents a game scene.
*/

#include "Manager/log_manager.h"
#include "Manager/setting_manager.h"
#include "Shape/full_rectangle.h"
#include "Shape/full_text.h"
#include "Shape/full_textbox.h"
#include <functional>
#include <map>
#include <memory>
#include <Shape/full_textinput.h>
#include <string>
#include <vector>

#ifndef SCREEN_H
#define SCREEN_H

enum Display { TitleScreen, NameScreen, StatScreen,
	GameScreen, MenuScreen, LevelScreen, InventoryScreen, SpellScreen, MapScreen, ShopScreen,
	LoadScreen, SpellAttackScreen, LogScreen, StatusScreen, SettingScreen, CustomScreen, MessageScreen, ExitScreen
};

enum ReusableButton { ExitButton, ConfirmButton, StatFulLButton, StatIncreaseButton, UseButton, DiscardButton,
ClearButton, };

enum Msg { NormalMsg, ErrorMsg, SuccessMsg };

class Screen : public Log_Manager, protected Setting_Manager {
protected:
	static bool loaded;
	bool exit_button = false;
	bool confirm_button = false;
	bool clearButton = false;
	bool show_bg = true;
	bool update = false;

	std::vector<Full_Text> texts;
	std::vector<Full_Text> hoverableTexts;
	std::vector<Full_Rectangle> rects;
	std::vector<Full_Textbox> textboxes;
	std::vector<Full_TextInput> textInputs;

	// Stat selection common UI:
	static Full_Text stat_full_txts[NUM_NON_CUR_STATS];
	static Full_Text stat_curr_txts[NUM_NON_CUR_STATS];
	static Full_Text stat_curr_arrows[NUM_NON_CUR_STATS * 2];

	static Full_Rectangle inv_sp_slots[MAX_INV_SPELL_SLOTS];

	static std::map<std::string, Full_Rectangle> map_rects;
	static std::map<std::string, Full_Text> map_txts;

	/*
	* Helper to setup shape.
	* 
	* Parameter:
	*	text: a string if it's a text. NULL otherwise.
	*	x: shape's x position.
	*	y: shape's y position.
	*	sx: shape's width.
	*	sy: shape's height.
	*	hoverable: true if shape is hoverable.
	*	override_theme: true if theme is no longer in effect.
	*/
	void setup_helper(const char* text, float x, float y, float sx, float sy,
		bool hoverable = true, bool override_theme = false);

	/*
	* Helper to setup textbox.
	*
	* Parameter:
	*	text: a string for the text.
	*	x: shape's x position.
	*	y: shape's y position.
	*	sx: shape's width.
	*	sy: shape's height.
	*	void (*func)(): function for click textbox callback.
	*	fontSize: text's size.
	*	fontOutline: text's outline size.
	*/
	void setupTextbox(const char* text, float x, float y, float sx, float sy, std::function<void()> func, float fontSize = 0, float fontOutline = 0);

	/*
	* Helper to setup hoverable and clickable texts.
	*
	* Parameter:
	*	text: a string for the text.
	*	x: text's x position.
	*	y: text's y position.
	*	void (*func)(): function for click textbox callback.
	*	fontSize: text's size.
	*	fontOutline: text's outline size.
	*/
	void setupHoverableText(const char* text, float x, float y, std::function<void()> func, float fontSize = 0, float fontOutline = 0);

	/*
	* Helper to setup hoverable and clickable texts.
	*/
	void setupTextInput(const char* defaultText, unsigned int length, float x, float y, float w, float h, InputValidation validation, float fontSize = 0.f, float fontOutline = 0.f);

	/**
	* Helper method to check mouse in shape.
	*
	* Parameter:
	*	element: true if shape is a rectangle. false for text.
	*	i: the index of where the shape is located.
	* 
	* Return:
	*	true if mouse is in shape.
	*/
	bool mouse_in_helper(bool element, unsigned int i);

	/**
	* Helper method to check mouse in a pre-defined button.
	*
	* Parameter:
	*	button: the button to check.
	* 
	* Return:
	*	true if mouse is in button.
	*/
	static bool mouse_in_button(ReusableButton button);

	/**
	* Check if mouse is in slow.
	*
	* Parameter:
	*	i: the index of where the slot is located.
	*
	* Return:
	*	true if mouse is in slot.
	*/
	bool mouse_in_slot(unsigned int i);

	/**
	* Helper method to check hover in a pre-defined button.
	*
	* Parameter:
	*	button: the button to check.
	*/
	static void hover_button(ReusableButton button);

	/**
	* Helper method to set hover status for an inventory slot.
	*
	* Parameter:
	*	i: the index of where the slow.
	*/
	void hover_slot(unsigned int i);

	/**
	* Returns to the previous screen.
	*
	* Parameter:
	*	screen: the screen to leave.
	*/
	static void return_to_prev_screen(Display screen);

	/**
	* Switches to another screen.
	*
	* Parameter:
	*	old_screen: the screen to leave.
	*	new_screen: the screen to enter.
	*	push: true to push old screen to history.
	*	clear: clear screen history if true.
	*/
	static void switch_screen(Display old_screen, Display new_screen, bool push, bool clear = false);

	/**
	* Set hover status of a textbox (text + rectangle).
	*
	* Parameter:
	*	i: the index of where the rectangle is located.
	*	j: the index of where the text is located.
	* 
	* Return:
	*	true if hovered over.
	*/
	bool hover_textbox(unsigned int i, int j);

	/**
	* Helper method to set hover status.
	* 
	* Parameter:
	*	element: true if shape is a rectangle. false for text.
	*	i: the index of where the shape is located.
	* 
	* Return:
	*	true if hovered over.
	*/
	bool hover_helper(bool element, unsigned int i);

	/**
	* Helper method to change the theme for a screen.
	*/
	virtual void change_theme();

public:
	static Display display;
	static std::vector<Display> prev_displays;

	const static unsigned int num_screens = 18;
	static bool visibilities[num_screens];
	static std::unique_ptr<Screen> screens[num_screens];

	/**
	* Constructor for Screen.
	* 
	* Parameter:
	*	exit_button: true to display an exit button.
	*	show_bg: true to display the bg.
	*	confirm_button: true to display a confirm button.
	*/
	Screen(bool exit_button = false, bool show_bg = true, bool confirm_button = false, bool clearButton = false);

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
	virtual bool click_event_handler();

	/**
	* Hover event handler for the screen.
	*/
	virtual void hover_event_handler();

	/**
	* Key press event handler for the screen.
	*/
	virtual void key_event_handler();

	/**
	* Performs setup work for Screen class.
	*/
	static void setup();

	/**
	* Show a dialog.
	*
	* Parameter:
	*	old_screen: the value of the old screen.
	*	new_screen: the value of the dialog.
	*/
	static void show_dialog(Display old_screen, Display new_screen);

	static void showMessage(Display old_screen, const char* newMsg, Msg category);

	/**
	* Change UI appearance according to settings.
	*/
	static void change_settings();

	/**
	* Getters for update.
	* 
	* Return:
	*	update value.
	*/
	bool get_update();

	void hover();

	static bool click();

	virtual void handleTextEvent();
};

#endif