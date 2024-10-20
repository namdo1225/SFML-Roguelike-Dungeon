/**
*
* File: setting_screen.cpp
* Description: Contain the implementation of the Setting_Screen class.
*/


#include "Screen/setting_screen.h"
#include "interface.h"
#include <string>

Setting_Screen::Setting_Screen() : Screen(31, 2, true) {
	light_mode = Setting_Manager::light;
	theme = Setting_Manager::theme;
	sfx_volume = Setting_Manager::sfx_volume;
	music_volume = Setting_Manager::music_volume;
	font = Setting_Manager::font;

	setup_helper(false, 0,   "Settings", 400.f, 10.f, 96.f, NULL);
	setup_helper(false, 1,      "Theme", 20.f, 150.f, NULL, NULL);
	setup_helper(false, 2, "Light Mode", 20.f, 200.f, NULL, NULL);
	setup_helper(false, 3,       "Font", 20.f, 250.f, NULL, NULL);
	setup_helper(false, 4, "SFX Volume", 20.f, 300.f, NULL, NULL);
	setup_helper(false, 5,      "Music", 20.f, 350.f, NULL, NULL);

	setup_helper(false, 6, "0", 300.f, 150.f, NULL, NULL);
	setup_helper(false, 7, "1", 350.f, 150.f, NULL, NULL);

	setup_helper(false, 8,  "Dark", 300.f, 200.f, NULL, NULL);
	setup_helper(false, 9, "Light", 400.f, 200.f, NULL, NULL);

	setup_helper(false, 10,  "Open Sans", 300.f, 250, NULL, NULL);
	setup_helper(false, 11, "Comic Neue", 500.f, 250.f, NULL, NULL);
	setup_helper(false, 12, "Montserrat", 700.f, 250.f, NULL, NULL);

	setup_helper(false, 13,   "0", 300.f, 300.f, NULL, NULL);
	setup_helper(false, 14,  "25", 350.f, 300.f, NULL, NULL);
	setup_helper(false, 15,  "50", 400.f, 300.f, NULL, NULL);
	setup_helper(false, 16,  "75", 450.f, 300.f, NULL, NULL);
	setup_helper(false, 17, "100", 500.f, 300.f, NULL, NULL);

	setup_helper(false, 18,   "0", 300.f, 350.f, NULL, NULL);
	setup_helper(false, 19,  "25", 350.f, 350.f, NULL, NULL);
	setup_helper(false, 20,  "50", 400.f, 350.f, NULL, NULL);
	setup_helper(false, 21,  "75", 450.f, 350.f, NULL, NULL);
	setup_helper(false, 22, "100", 500.f, 350.f, NULL, NULL);

	setup_helper(false, 23,                           "Selected", 1000.f, 100.f, NULL, NULL);
	setup_helper(false, 24,        std::to_string(theme).c_str(), 1000.f, 150.f, NULL, NULL);
	setup_helper(false, 25,        light_mode ? "Light" : "Dark", 1000.f, 200.f, NULL, NULL);
	setup_helper(false, 26, Font_Manager::get_selected().getInfo().family.c_str(), 1000.f, 250.f, NULL, NULL);
	setup_helper(false, 27,   std::to_string(sfx_volume).c_str(), 1000.f, 300.f, NULL, NULL);
	setup_helper(false, 28, std::to_string(music_volume).c_str(), 1000.f, 350.f, NULL, NULL);

	setup_helper(false, 29, "Theme Preview: abcdefghijklmnopqrstuvwxyz, ABCDEFGHIJKLMNOPQRSTUVWXYZ", 25.f, 510.f,  NULL, NULL);
	setup_helper(true,   0,                                                                    NULL, 20.f, 500.f, 1150.f, 45.f);

	setup_helper(false, 30, "Save", 30.f, 610.f, NULL, NULL);
	setup_helper(true,   1,   NULL, 20.f, 600.f, 80.f, 45.f);

	texts[0].setThemeAndHover(Setting_Manager::theme, false);
	texts[1].setThemeAndHover(Setting_Manager::theme, false);
	texts[2].setThemeAndHover(Setting_Manager::theme, false);
	texts[3].setThemeAndHover(Setting_Manager::theme, false);
	texts[4].setThemeAndHover(Setting_Manager::theme, false);
	texts[5].setThemeAndHover(Setting_Manager::theme, false);
	texts[23].setThemeAndHover(Setting_Manager::theme, false);
	texts[24].setThemeAndHover(Setting_Manager::theme, false);
	texts[25].setThemeAndHover(Setting_Manager::theme, false);
	texts[26].setThemeAndHover(Setting_Manager::theme, false);
	texts[27].setThemeAndHover(Setting_Manager::theme, false);
	texts[28].setThemeAndHover(Setting_Manager::theme, false);
}

void Setting_Screen::click_event_handler() {
	if (mouse_in_button(ExitButton)) {
		light_mode = Setting_Manager::light;
		theme = Setting_Manager::theme;
		sfx_volume = Setting_Manager::sfx_volume;
		music_volume = Setting_Manager::music_volume;
		font = Setting_Manager::font;
		texts[24].setString(std::to_string(theme).c_str());
		texts[25].setString(light_mode ? "Light" : "Dark");
		texts[26].setString(Font_Manager::get_selected().getInfo().family.c_str());
		texts[27].setString(std::to_string(sfx_volume).c_str());
		texts[28].setString(std::to_string(music_volume).c_str());

		return_to_prev_screen(SettingScreen);
	}
	else if (mouse_in_helper(false, 6)) {
		theme = 0;
		texts[24].setString(std::to_string(theme).c_str());
	}
	else if (mouse_in_helper(false, 7)) {
		theme = 1;
		texts[24].setString(std::to_string(theme).c_str());
	}
	else if (mouse_in_helper(false, 8)) {
		light_mode = false;
		texts[25].setString("Dark");
	}
	else if (mouse_in_helper(false, 9)) {
		light_mode = true;
		texts[25].setString("Light");

	}
	else if (mouse_in_helper(true, 1) && (Setting_Manager::light != light_mode ||
		Setting_Manager::theme != theme || Setting_Manager::sfx_volume != sfx_volume ||
		Setting_Manager::music_volume != music_volume || Setting_Manager::font != font)) {
		Setting_Manager::light = light_mode;
		Setting_Manager::theme = theme;
		Setting_Manager::sfx_volume = sfx_volume;
		Setting_Manager::music_volume = music_volume;
		Setting_Manager::font = font;
		change_settings();
	}
	for (unsigned int i = 10; i < 13; i++)
		if (mouse_in_helper(false, i)) {
			font = i - 10;
			texts[26].setString(texts[i].getString());
		}
	for (unsigned int i = 13; i < 18; i++)
		if (mouse_in_helper(false, i)) {
			std::string string = texts[i].getString();
			sfx_volume = atoi(string.c_str());
			texts[27].setString(string);
		}
	for (unsigned int i = 18; i < 23; i++)
		if (mouse_in_helper(false, i)) {
			std::string string = texts[i].getString();
			music_volume = atoi(string.c_str());
			texts[28].setString(string);
		}
}

void Setting_Screen::hover_event_handler() {
	for (unsigned int i = 5; i < 23; i++) {
		bool in = mouse_in_helper(false, i);
		bool hover = texts[i].getHover();

		if (in && !hover)
			texts[i].highlight();
		else if (!in && hover)
			texts[i].highlight(false);
	}

	hover_textbox(0, 29);
	hover_textbox(1, 30);

	hover_button(ExitButton);
}

void Setting_Screen::draw() {
	texts[28].setString(std::to_string(Setting_Manager::music_volume).c_str());
	Screen::draw();
}
