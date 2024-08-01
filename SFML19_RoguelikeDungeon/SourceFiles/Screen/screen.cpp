/**
*
* File: screen.cpp
* Description: Contain the implementation of the Screen class.
*/


#include "Screen/screen.h"
#include "Manager/font_manager.h"
#include "Manager/audio_manager.h"

Display Screen::display = TitleScreen;
std::vector<Display> Screen::prev_displays = {};

const unsigned int Screen::num_screens;

bool Screen::visibilities[num_screens] = {true, false, false, false, false, 
false, false, false, false, false, false, false, false, false };

std::unique_ptr<Screen> Screen::screens[num_screens];

bool Screen::loaded = false;

Full_Text Screen::stat_full_txts[NUM_NON_CUR_STATS] = {
	Full_Text(280.f, 250.f, 24.f, FULL_STATS[0]),
	Full_Text(280.f, 300.f, 24.f, FULL_STATS[1]),
	Full_Text(280.f, 350.f, 24.f, FULL_STATS[2]),
	Full_Text(280.f, 400.f, 24.f, FULL_STATS[3]),
	Full_Text(280.f, 450.f, 24.f, FULL_STATS[4]),
	Full_Text(280.f, 500.f, 24.f, FULL_STATS[5]),
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
Full_Rectangle Screen::inv_sp_slots[MAX_INV_SPELL_SLOTS];

std::map<std::string, Full_Rectangle> Screen::map_rects;
std::map<std::string, Full_Text> Screen::map_txts;

Screen::Screen(unsigned int txt, unsigned int rect,
	bool exit_button, bool show_bg, bool confirm_button, bool text_handler_enabled):
	exit_button(exit_button), show_bg(show_bg), confirm_button(confirm_button), text_handler_enabled(text_handler_enabled) {
	if (!loaded)
		setup();

	while (txt > 0) {
		texts.push_back(Full_Text());
		unsigned int i = texts.size() - 1;
		texts[i].setFont(Font_Manager::get_selected());
		texts[i].setCharacterSize(24);
		texts[i].setStyle(sf::Text::Bold);
		txt--;
	}

	while (rect > 0) {
		rects.push_back(Full_Rectangle());
		unsigned int i = rects.size() - 1;
		rects[i].setOutlineThickness(3.f);
		rect--;
	}
}

void Screen::setup() {
	loaded = true;
	window.create(sf::VideoMode(1200, 800), "RE: Dungeon");
	window.setKeyRepeatEnabled(false);

	for (unsigned int i = 0; i < MAX_INV_SPELL_SLOTS; i++) {
		inv_sp_slots[i] = Full_Rectangle(200 + ((i % 6) * 80),
			240 + ((i / 6) * 80),
			60.f,
			60.f);
	}

	map_rects["background"] = Full_Rectangle(-10, -10, 1210, 810, false);

	map_rects["exit"] = Full_Rectangle(1120, 10, 50, 50);
	map_txts["exit"] = Full_Text(1130.f, 5.f, 48, "X");

	map_rects["confirm"] = Full_Rectangle(500.f, 700.f, 150.f, 50.f);
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

	map_rects["inv_sp_discard"] = Full_Rectangle(420.f, 660.f, 160.f, 80.f);
	map_txts["inv_sp_discard"] = Full_Text(430.f, 670.f, 36.f, "Discard");

	map_rects["inv_sp_cur_slot"] = Full_Rectangle(-100.f, -100.f, 70.f, 70.f, 0, false, sf::Color::Red, sf::Color::Red);

	map_txts["inv_sp_gold"] = Full_Text(750.f, 30.f, 24.f, "Golds:");
	map_txts["inv_sp_gold_amount"] = Full_Text(850.f, 30.f, 24.f, "0");
}

void Screen::draw() {
	if (show_bg)
		window.draw(map_rects["background"]);

	for (Full_Rectangle rect : rects)
		window.draw(rect);
	for (Full_Text text : texts)
		window.draw(text);

	if (text_handler_enabled) {
		window.draw(map_rects["clear"]);
		window.draw(map_txts["clear"]);
		window.draw(map_rects["name"]);
		window.draw(map_txts["name"]);
	}

	if (confirm_button) {
		window.draw(map_rects["confirm"]);
		window.draw(map_txts["confirm"]);
	}

	if (exit_button) {
		window.draw(map_rects["exit"]);
		window.draw(map_txts["exit"]);
	}
}

void Screen::setup_helper(unsigned int i, const char* text, float x, float y, float sx, float sy,
	bool hoverable, bool override_theme) {
	text == NULL ? rects[i].setPhysical(x, y, sx, sy) : texts[i].setPhysical(x, y, text, sx, sy);
	text == NULL ? rects[i].setThemeAndHover(hoverable, override_theme) :
		texts[i].setThemeAndHover(hoverable, override_theme);
}

bool Screen::mouse_in_helper(bool element, unsigned int i) {
	return (element && rects[i].getGlobalBounds().contains(sf::Vector2f(x, y))) ||
		(!element && texts[i].getGlobalBounds().contains(sf::Vector2f(x, y)));
}

bool Screen::mouse_in_button(ReusableButton button) {
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

void Screen::hover_button(ReusableButton button) {
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

	bool in = mouse_in_button(button);
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

void Screen::return_to_prev_screen(Display screen) {
	Display prev = prev_displays[prev_displays.size() - 1];
	prev_displays.pop_back();

	visibilities[screen] = false;
	visibilities[prev] = true;
	display = prev;
}

void Screen::switch_screen(Display old_screen, Display new_screen, bool push, bool clear) {
	visibilities[old_screen] = false;
	visibilities[new_screen] = true;
	display = new_screen;
	if (push)
		prev_displays.push_back(old_screen);
	if (clear)
		prev_displays.clear();
}

void Screen::show_dialog(Display old_screen, Display new_screen) {
	visibilities[new_screen] = true;
	display = new_screen;
	prev_displays.push_back(old_screen);
}

bool Screen::hover_textbox(unsigned int i, int j) {
	bool in = mouse_in_helper(true, i);
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
	Audio_Manager::set_music_volume(Setting_Manager::music_volume);
	Audio_Manager::set_sfx_volume(Setting_Manager::sfx_volume);
	Font_Manager::set(Setting_Manager::font);
	
	for (unsigned int i = 0; i < num_screens; i++) {
		if (screens[i].get() != NULL) {
			screens[i]->change_theme();
			for (Full_Rectangle& rect : screens[i]->rects)
				rect.flip_theme();
			for (Full_Text& text : screens[i]->texts) {
				text.flip_theme();
				text.setFont(Font_Manager::get_selected());
			}
		}
	}

	std::map<std::string, Full_Rectangle>::iterator it_r = map_rects.begin();
	while (it_r != map_rects.end()) {
		it_r->second.flip_theme();
		it_r++;
	}

	std::map<std::string, Full_Text>::iterator it_t = map_txts.begin();
	while (it_t != map_txts.end()) {
		it_t->second.flip_theme();
		it_t->second.setFont(Font_Manager::get_selected());
		it_t++;
	}

	for (Full_Text& text : stat_full_txts) {
		text.flip_theme();
		text.setFont(Font_Manager::get_selected());
	}

	for (Full_Text& text : stat_curr_txts) {
		text.flip_theme();
		text.setFont(Font_Manager::get_selected());
	}

	for (Full_Text& text : stat_curr_arrows) {
		text.flip_theme();
		text.setFont(Font_Manager::get_selected());
	}

	for (Full_Rectangle& rect : inv_sp_slots)
		rect.flip_theme();

	for (Full_Text& text : logs) {
		text.flip_theme();
		text.setFont(Font_Manager::get_selected());
	}
}

void Screen::key_event_handler() {
}

void Screen::text_event_handler() {
	if (text_handler_enabled)
		text_event_helper();
}

void Screen::text_event_helper() {
	char letter{ static_cast<char>(event.text.unicode) };
	sf::String cur_string = map_txts["name"].getString();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && !map_txts["name"].getString().isEmpty())
		map_txts["name"].setString(cur_string.substring(0, cur_string.getSize() - 1));
	else if (cur_string.getSize() < 20 && letter > ' ' && event.text.unicode < 128 && letter != '\\' && letter != '=' &&
		letter != '$' && letter != '*' && letter != '|' && letter != '~' && letter != '.' && letter != '"' && letter != '\''
		&& letter != ';')
		map_txts["name"].setString(cur_string + letter);
}

bool Screen::mouse_in_slot(unsigned int i) {
	return inv_sp_slots[i].getGlobalBounds().contains(sf::Vector2f(x, y));
}

void Screen::hover_slot(unsigned int i) {
	bool in = mouse_in_slot(i);
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

void Screen::mouse_event_handler() {
}

bool Screen::hover_helper(bool element, unsigned int i) {
	bool in = mouse_in_helper(element, i);
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

void Screen::update_draw() {
}

void Screen::change_theme() {
}