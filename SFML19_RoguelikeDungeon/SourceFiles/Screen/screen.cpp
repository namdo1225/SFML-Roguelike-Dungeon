/**
*
* File: screen.cpp
* Description: Contain the implementation of the Screen class.
*/


#include "Manager/audio_manager.h"
#include "Manager/font_manager.h"
#include "Screen/screen.h"
#include <functional>
#include <Manager/setting_manager.h>
#include <map>
#include <memory>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <Shape/full_rectangle.h>
#include <Shape/full_text.h>
#include <Shape/full_textbox.h>
#include <Shape/full_textinput.h>
#include <stat.h>
#include <string>
#include <utility>
#include <vector>
#include <Tool/spell.h>
#include <Tool/item.h>
#include <Tool/special.h>

Display Screen::display = TitleScreen;
std::vector<Display> Screen::prev_displays = {};

const unsigned int Screen::num_screens;

bool Screen::visibilities[num_screens] = {true, false, false, false, false, 
false, false, false, false, false, false, false, false, false };

std::unique_ptr<Screen> Screen::screens[num_screens];

bool Screen::loaded = false;

Full_Text Screen::stat_full_txts[NUM_NON_CUR_STATS] = {
	Full_Text(280.f, 250.f, 24.f, StatConst::FULL_STATS[0]),
	Full_Text(280.f, 300.f, 24.f, StatConst::FULL_STATS[1]),
	Full_Text(280.f, 350.f, 24.f, StatConst::FULL_STATS[2]),
	Full_Text(280.f, 400.f, 24.f, StatConst::FULL_STATS[3]),
	Full_Text(280.f, 450.f, 24.f, StatConst::FULL_STATS[4]),
	Full_Text(280.f, 500.f, 24.f, StatConst::FULL_STATS[5]),
};
Full_Text Screen::stat_curr_txts[NUM_NON_CUR_STATS] = {
	Full_Text(590.f, 250.f, 24.f, "10"),
	Full_Text(590.f, 300.f, 24.f, "5"),
	Full_Text(590.f, 350.f, 24.f, "0"),
	Full_Text(590.f, 400.f, 24.f, "0"),
	Full_Text(590.f, 450.f, 24.f, "0"),
	Full_Text(590.f, 500.f, 24.f, "0"),
};
Full_Text Screen::stat_curr_arrows[NUM_NON_CUR_STATS * 2] = {
	Full_Text(520.f, 250.f, 24.f, "<"),
	Full_Text(520.f, 300.f, 24.f, "<"),
	Full_Text(520.f, 350.f, 24.f, "<"),
	Full_Text(520.f, 400.f, 24.f, "<"),
	Full_Text(520.f, 450.f, 24.f, "<"),
	Full_Text(520.f, 500.f, 24.f, "<"),
	Full_Text(660.f, 250.f, 24.f, ">"),
	Full_Text(660.f, 300.f, 24.f, ">"),
	Full_Text(660.f, 350.f, 24.f, ">"),
	Full_Text(660.f, 400.f, 24.f, ">"),
	Full_Text(660.f, 450.f, 24.f, ">"),
	Full_Text(660.f, 500.f, 24.f, ">"),
};

const unsigned int Screen::MAX_INV_SPELL_SLOTS;
std::vector<Full_Rectangle> Screen::inv_sp_slots;

std::map<std::string, Full_Rectangle> Screen::map_rects;
std::map<std::string, Full_Text> Screen::map_txts;

Screen::Screen(bool exitButton, bool showBG, bool confirmButton, bool clearButton):
	exitButton(exitButton), showBG(showBG), confirmButton(confirmButton), clearButton(clearButton) {
	if (!loaded)
		setup();
}

void Screen::setup() {
	loaded = true;
	window.create(sf::VideoMode(DEFAULT_SCREEN_X, DEFAULT_SCREEN_Y), "RE: Dungeon");

	size_t SLOTS = MAX_INV_SPELL_SLOTS;
	SLOTS = std::max(SLOTS, Spell::spells.size());
	SLOTS = std::max(SLOTS, Item::items.size());
	SLOTS = std::max(SLOTS, Special::specials.size());

	for (unsigned int i = 0; i < SLOTS; i++) {
		inv_sp_slots.push_back(Full_Rectangle(200 + ((i % 6) * 80),
			240 + ((i / 6) * 80),
			60.f,
			60.f));
	}

	map_rects["background"] = Full_Rectangle(-10, -10, 1210, 810, false);

	map_rects["exit"] = Full_Rectangle(1120, 10, 50, 50);
	map_txts["exit"] = Full_Text(1130.f, 5.f, 48, "X");

	map_rects["confirm"] = Full_Rectangle(500.f, 700.f, 160.f, 50.f);
	map_txts["confirm"] = Full_Text(530.f, 710.f, 24, "Confirm");

	map_rects["name"] = Full_Rectangle(350.f, 240.f, 470.f, 50.f);
	map_txts["name"] = Full_Text(360.f, 250.f, 24.f, "Player");

	map_rects["clear"] = Full_Rectangle(700.f, 700.f, 150.f, 50.f);
	map_txts["clear"] = Full_Text(745.f, 710.f, 24.f, "Clear");

	map_txts["stat_guide"] = Full_Text(20.f, 20.f, 42, "Please allocate the player's stats. You have a limited \nnumber of stat points, so balance your stats carefully.\nYou can mess up your player if not done correctly.");
	map_txts["stat_left_guide"] = Full_Text(200.f, 190.f, 36, "Stat points left:");
	map_txts["stat_left"] = Full_Text(580.f, 190.f, 36, "10");

	map_rects["inv_sp_use"] = Full_Rectangle(620.f, 660.f, 100.f, 80.f);
	map_txts["inv_sp_use"] = Full_Text(630.f, 670.f, 36.f, "Use");

	map_rects["inv_sp_desc"] = Full_Rectangle(750.f, 140.f, 400.f, 600.f);
	map_txts["inv_sp_desc"] = Full_Text(875.f, 100.f, 24.f, "Description");
	map_txts["inv_sp_detail"] = Full_Text(760.f, 150.f, 20.f, "");

	map_rects["inv_sp_discard"] = Full_Rectangle(420.f, 660.f, 160.f, 80.f);
	map_txts["inv_sp_discard"] = Full_Text(430.f, 670.f, 36.f, "Discard");

	map_rects["inv_sp_cur_slot"] = Full_Rectangle(-100.f, -100.f, 70.f, 70.f, 0, false, sf::Color::Red, sf::Color::Red);

	map_txts["inv_sp_gold"] = Full_Text(750.f, 30.f, 24.f, "Golds:");
	map_txts["inv_sp_gold_amount"] = Full_Text(850.f, 30.f, 24.f, "0");
}

void Screen::draw() {
	if (showBG)
		window.draw(map_rects["background"]);

	for (Full_Rectangle rect : rects)
		window.draw(rect);
	for (Full_Textbox boxes : textboxes) {
		window.draw(boxes.rect);
		window.draw(boxes.text);
	}
	for (Full_TextInput input : textInputs) {
		window.draw(input.rect);
		window.draw(input.text);
	}
	for (Full_Text text : hoverableTexts)
		window.draw(text);
	for (Full_Text text : texts)
		window.draw(text);

	if (clearButton) {
		window.draw(map_rects["clear"]);
		window.draw(map_txts["clear"]);
	}

	if (confirmButton) {
		window.draw(map_rects["confirm"]);
		window.draw(map_txts["confirm"]);
	}

	if (exitButton) {
		window.draw(map_rects["exit"]);
		window.draw(map_txts["exit"]);
	}
}

void Screen::textRectH(const char* text, float x, float y, float sx, float sy,
	bool hoverable, bool override_theme) {
	if (text == NULL) {
		rects.push_back(Full_Rectangle());
		unsigned int i = rects.size() - 1;
		rects[i].setPhysical(x, y, sx, sy);
		rects[i].setThemeAndHover(hoverable, override_theme);
	}
	else {
		texts.push_back(Full_Text());
		unsigned int i = texts.size() - 1;
		texts[i].setPhysical(x, y, text, sx, sy);
		texts[i].setThemeAndHover(hoverable, override_theme);
	}		
}

void Screen::textboxH(const char* text, float x, float y, float sx, float sy, std::function<void()> func, float fontSize, float fontOutline) {
	textboxes.push_back(Full_Textbox(text, x, y, sx, sy, func, fontSize, fontOutline));
}

void Screen::hoverableTextH(const char* text, float x, float y, std::function<void()> func, float fontSize, float fontOutline) {
	hoverableTexts.push_back(Full_Text(x, y, fontSize, text, true, false, func));
	hoverableTexts[hoverableTexts.size() - 1].setPhysical(x, y, text, fontSize, fontOutline);
}

void Screen::textInputH(const char* defaultText, unsigned int length, float x, float y, float w, float h, InputValidation validation, float fontSize, float fontOutline) {
	textInputs.push_back(Full_TextInput(defaultText, length, x, y, w, h, validation, fontSize, fontOutline));
}

bool Screen::mouseInH(bool element, unsigned int i) {
	return (element && rects[i].getGlobalBounds().contains(sf::Vector2f(x, y))) ||
		(!element && texts[i].getGlobalBounds().contains(sf::Vector2f(x, y)));
}

bool Screen::mouseInButton(ReusableButton button) {
	switch (button) {
	case ExitButton:
		return map_rects["exit"].getGlobalBounds().contains(sf::Vector2f(x, y));
	case ConfirmButton:
		return map_rects["confirm"].getGlobalBounds().contains(sf::Vector2f(x, y));
	case UseButton:
		return map_rects["inv_sp_use"].getGlobalBounds().contains(sf::Vector2f(x, y));
	case DiscardButton:
		return map_rects["inv_sp_discard"].getGlobalBounds().contains(sf::Vector2f(x, y));
	case ClearButton:
		return map_rects["clear"].getGlobalBounds().contains(sf::Vector2f(x, y));
	}
	return false;
}

void Screen::hoverButton(ReusableButton button) {
	Full_Rectangle* button_rect = &map_rects["exit"];
	Full_Text* button_txt = &map_txts["exit"];

	switch (button) {
	case ConfirmButton:
		button_rect = &map_rects["confirm"];
		button_txt = &map_txts["confirm"];
		break;
	case UseButton:
		button_rect = &map_rects["inv_sp_use"];
		button_txt = &map_txts["inv_sp_use"];
		break;
	case DiscardButton:
		button_rect = &map_rects["inv_sp_discard"];
		button_txt = &map_txts["inv_sp_discard"];
		break;
	case ClearButton:
		button_rect = &map_rects["clear"];
		button_txt = &map_txts["clear"];
		break;
	}

	bool in = mouseInButton(button);
	bool hover = button_rect->getHover();

	if (in && !hover) {
		button_rect->highlight();
		button_txt->highlight();
	}
	else if (!in && hover) {
		button_rect->highlight(false);
		button_txt->highlight(false);
	}
}

void Screen::goToPrevScreen(Display screen) {
	Display prev = prev_displays[prev_displays.size() - 1];
	prev_displays.pop_back();

	visibilities[screen] = false;
	visibilities[prev] = true;
	display = prev;
}

void Screen::switchScreen(Display oldScreen, Display newScreen, bool push, bool clear) {
	visibilities[oldScreen] = false;
	visibilities[newScreen] = true;
	display = newScreen;
	if (push)
		prev_displays.push_back(oldScreen);
	if (clear)
		prev_displays.clear();
}

void Screen::openDialog(Display oldScreen, Display newScreen) {
	visibilities[newScreen] = true;
	display = newScreen;
	prev_displays.push_back(oldScreen);
}

void Screen::openMessage(Display oldScreen, const char* newMsg, Msg category) {
	visibilities[MessageScreen] = true;
	display = MessageScreen;
	prev_displays.push_back(oldScreen);
	screens[MessageScreen]->texts[0].setString(newMsg);
	switch (category) {
	case NormalMsg:
		screens[MessageScreen]->rects[1].setOutlineColor(sf::Color::Cyan);
		screens[MessageScreen]->texts[0].setFillColor(sf::Color::Cyan);
		break;
	case ErrorMsg:
		screens[MessageScreen]->rects[1].setOutlineColor(sf::Color::Red);
		screens[MessageScreen]->texts[0].setFillColor(sf::Color::Red);
		break;
	case SuccessMsg:
		screens[MessageScreen]->rects[1].setOutlineColor(sf::Color::Green);
		screens[MessageScreen]->texts[0].setFillColor(sf::Color::Green);
		break;
	}
}

bool Screen::hoverTextRect(unsigned int i, int j) {
	bool in = mouseInH(true, i);
	bool hover = rects[i].getHover();

	if (in && !hover) {
		rects[i].highlight();
		if (j != -1)
			texts[j].highlight();
		return true;
	}
	else if (!in && hover) {
		rects[i].highlight(false);
		if (j != -1)
			texts[j].highlight(false);
	}
	return false;
}

void Screen::change_settings() {
	Setting_Manager::save();
	Audio_Manager::setMusicVolume(Setting_Manager::music_volume);
	Audio_Manager::setSFXVolume(Setting_Manager::sfxVolume);
	Font_Manager::set(Setting_Manager::font);
	
	for (unsigned int i = 0; i < num_screens; i++) {
		if (screens[i].get() != NULL) {
			screens[i]->changeTheme();
			for (Full_Rectangle& rect : screens[i]->rects)
				rect.changeTheme();
			for (Full_Text& text : screens[i]->texts) {
				text.changeTheme();
				text.setFont(Font_Manager::get_selected());
			}
			for (Full_Text& text : screens[i]->hoverableTexts) {
				text.changeTheme();
				text.setFont(Font_Manager::get_selected());
			}
			for (Full_Textbox& textbox : screens[i]->textboxes) {
				textbox.text.changeTheme();
				textbox.text.setFont(Font_Manager::get_selected());
				textbox.rect.changeTheme();
			}
		}
	}

	std::map<std::string, Full_Rectangle>::iterator it_r = map_rects.begin();
	while (it_r != map_rects.end()) {
		it_r->second.changeTheme();
		it_r++;
	}

	std::map<std::string, Full_Text>::iterator it_t = map_txts.begin();
	while (it_t != map_txts.end()) {
		it_t->second.changeTheme();
		it_t->second.setFont(Font_Manager::get_selected());
		it_t++;
	}

	for (Full_Text& text : stat_full_txts) {
		text.changeTheme();
		text.setFont(Font_Manager::get_selected());
	}

	for (Full_Text& text : stat_curr_txts) {
		text.changeTheme();
		text.setFont(Font_Manager::get_selected());
	}

	for (Full_Text& text : stat_curr_arrows) {
		text.changeTheme();
		text.setFont(Font_Manager::get_selected());
	}

	for (Full_Rectangle& rect : inv_sp_slots)
		rect.changeTheme();

	for (Full_Text& text : logs) {
		text.changeTheme();
		text.setFont(Font_Manager::get_selected());
	}
}

void Screen::handleKeyEvent() {
}

bool Screen::mouseInSlot(unsigned int i) {
	return inv_sp_slots[i].getGlobalBounds().contains(sf::Vector2f(slotX, slotY));
}

void Screen::hoverSlot(unsigned int i) {
	bool in = mouseInSlot(i);
	bool hover = inv_sp_slots[i].getHover();

	if (in && !hover) {
		inv_sp_slots[i].highlight();
		inv_sp_slots[i].highlight();
	}
	else if (!in && hover) {
		inv_sp_slots[i].highlight(false);
		inv_sp_slots[i].highlight(false);
	}
}

void Screen::handleMouseEvent() {}

bool Screen::handleClickEvent() {
	return false;
}

void Screen::handleHoverEvent() {}

bool Screen::hoverH(bool element, unsigned int i) {
	bool in = mouseInH(element, i);
	bool hover = element ? rects[i].getHover() : texts[i].getHover();

	if (in && !hover) {
		element ? rects[i].highlight() : texts[i].highlight();
		return true;
	}
	else if (!in && hover) {
		element ? rects[i].highlight(false) : texts[i].highlight(false);
	}
	return false;
}

bool Screen::get_update() {
	return update;
}

void Screen::updateDraw() {
}

void Screen::changeTheme() {
}

void Screen::hover() {
	if (exitButton)
		hoverButton(ExitButton);

	if (confirmButton)
		hoverButton(ConfirmButton);

	if (clearButton)
		hoverButton(ClearButton);

	for (unsigned int i = 0; i < screens[display]->hoverableTexts.size(); i++)
		screens[display]->hoverableTexts[i].hover();

	for (unsigned int i = 0; i < screens[display]->textboxes.size(); i++)
		screens[display]->textboxes[i].hover();
}

bool Screen::click() {
	for (int i = screens[display]->hoverableTexts.size() - 1; i >= 0; i--)
		if (screens[display]->hoverableTexts[i].click())
			return true;

	for (int i = screens[display]->textboxes.size() - 1; i >= 0; i--)
		if (screens[display]->textboxes[i].click())
			return true;

	for (int i = screens[display]->textInputs.size() - 1; i >= 0; i--)
		if (screens[display]->textInputs[i].click())
			return true;

	return false;
}

void Screen::handleTextEvent() {
	for (unsigned int i = 0; i < textInputs.size(); i++)
		textInputs[i].handleTextEvent();
}
