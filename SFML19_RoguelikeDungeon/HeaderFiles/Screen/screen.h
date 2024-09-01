#include "Manager/log_manager.h"
#include "Manager/setting_manager.h"
#include "Shape/full_rectangle.h"
#include "Shape/full_text.h"
#include "Shape/full_textbox.h"
#include <functional>
#include <Manager/sf_manager.h>
#include <map>
#include <memory>
#include <Shape/full_textinput.h>
#include <string>
#include <vector>

#ifndef SCREEN_H
#define SCREEN_H

/**
* Enum for all available screens in the program.
*/
enum Display { TitleScreen, NameScreen, StatScreen,
	GameScreen, MenuScreen, LevelScreen, InventoryScreen, SpellScreen, MapScreen, ShopScreen,
	LoadScreen, SpellAttackScreen, LogScreen, StatusScreen, SettingScreen, CustomScreen, MessageScreen, ExitScreen
};

/**
* Enum for all reusable buttons for the screen.
*/
enum ReusableButton { ExitButton, ConfirmButton, StatFulLButton, StatIncreaseButton, UseButton, DiscardButton,
ClearButton, };

/**
* Enum for message types for the MessageScreen.
*/
enum Msg { NormalMsg, ErrorMsg, SuccessMsg };

/**
* Contain the declaration of the Screen class, which represents a game scene.
*/
class Screen : public Log_Manager, public SF_Manager, protected Setting_Manager {
protected:
	static bool loaded;
	bool exitButton = false;
	bool confirmButton = false;
	bool clearButton = false;
	bool showBG = true;

	std::vector<Full_Text> texts;
	std::vector<Full_Text> hoverableTexts;
	std::vector<Full_Rectangle> rects;
	std::vector<Full_Textbox> textboxes;
	std::vector<Full_TextInput> textInputs;

	// Stat selection common UI:
	static Full_Text stat_full_txts[NUM_NON_CUR_STATS];
	static Full_Text stat_curr_txts[NUM_NON_CUR_STATS];
	static Full_Text stat_curr_arrows[NUM_NON_CUR_STATS * 2];

	static std::vector<Full_Rectangle> inv_sp_slots;

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
	void textRectH(const char* text, float x, float y, float sx, float sy,
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
	void textboxH(const char* text, float x, float y, float sx, float sy, std::function<void()> func, float fontSize = 0, float fontOutline = 0);

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
	void hoverableTextH(const char* text, float x, float y, std::function<void()> func, float fontSize = 0, float fontOutline = 0);

	/*
	* Helper to setup hoverable and clickable texts.
	*/
	void textInputH(const char* defaultText, unsigned int length, float x, float y, float w, float h, InputValidation validation, float fontSize = 0.f, float fontOutline = 0.f);

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
	bool mouseInH(bool element, unsigned int i);

	/**
	* Helper method to check mouse in a pre-defined button.
	*
	* Parameter:
	*	button: the button to check.
	* 
	* Return:
	*	true if mouse is in button.
	*/
	static bool mouseInButton(ReusableButton button);

	/**
	* Check if mouse is in slow.
	*
	* Parameter:
	*	i: the index of where the slot is located.
	*
	* Return:
	*	true if mouse is in slot.
	*/
	bool mouseInSlot(unsigned int i);

	/**
	* Helper method to check hover in a pre-defined button.
	*
	* Parameter:
	*	button: the button to check.
	*/
	static void hoverButton(ReusableButton button);

	/**
	* Helper method to set hover status for an inventory slot.
	*
	* Parameter:
	*	i: the index of where the slow.
	*/
	void hoverSlot(unsigned int i);

	/**
	* Returns to the previous screen.
	*
	* Parameter:
	*	screen: the screen to leave.
	*/
	static void goToPrevScreen(Display screen);

	/**
	* Switches to another screen.
	*
	* Parameter:
	*	oldScreen: the screen to leave.
	*	newScreen: the screen to enter.
	*	push: true to push old screen to history.
	*	clear: clear screen history if true.
	*/
	static void switchScreen(Display oldScreen, Display newScreen, bool push, bool clear = false);

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
	bool hoverTextRect(unsigned int i, int j);

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
	bool hoverH(bool element, unsigned int i);

	/**
	* Helper method to change the theme for a screen.
	*/
	virtual void changeTheme();

public:
	bool update = false;

	static Display display;
	static std::vector<Display> prev_displays;

	const static unsigned int num_screens = 18;
	static bool visibilities[num_screens];
	static std::unique_ptr<Screen> screens[num_screens];

	/**
	* Constructor for Screen.
	* 
	* Parameter:
	*	exitButton: true to display an exit button.
	*	showBG: true to display the bg.
	*	confirmButton: true to display a confirm button.
	*	clearButton: true to draw the clear button.
	*/
	Screen(bool exitButton = false, bool showBG = true, bool confirmButton = false, bool clearButton = false);

	/**
	* Draw the screen.
	*/
	virtual void draw();

	/**
	* Updates dynamic texts for a screen.
	*/
	virtual void updateDraw();

	/**
	* Mouse event handler for the screen.
	*/
	virtual void handleMouseEvent();

	/**
	* Click event handler for the screen.
	*/
	virtual bool handleClickEvent();

	/**
	* Hover event handler for the screen.
	*/
	virtual void handleHoverEvent();

	/**
	* Key press event handler for the screen.
	*/
	virtual void handleKeyEvent();

	/**
	* Performs setup work for Screen class.
	*/
	static void setup();

	/**
	* Show a dialog.
	*
	* Parameter:
	*	oldScreen: the value of the old screen.
	*	newScreen: the value of the dialog.
	*/
	static void openDialog(Display oldScreen, Display newScreen);

	static void openMessage(Display oldScreen, const char* newMsg, Msg category);

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

	/**
	* General hover function to call hover event on as many visible elements as possible.
	*/
	void hover();

	/**
	* General click function to call click event on as many visible elements as possible.
	*/
	static bool click();

	/**
	* General function to handle text event.
	*/
	virtual void handleTextEvent();
};

#endif