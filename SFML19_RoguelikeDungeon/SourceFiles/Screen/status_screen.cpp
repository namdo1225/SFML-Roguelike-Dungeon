/**
*
* File: status_screen.cpp
* Description: Contain the implementation of the Status_Screen class.
* 
*/

#include "Manager/game_manager.h"
#include "Screen/status_screen.h"
#include <effect.h>
#include <format>
#include <Screen/screen.h>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <Shape/full_text.h>
#include <stat.h>
#include <string>
#include <vector>

sf::View Status_Screen::viewEffect{ sf::FloatRect(0, 0, 1200, 800) };
Full_Text Status_Screen::effect_txt = Full_Text(800.f, 150.f, 20.f, "");

Status_Screen::Status_Screen() : Screen(true, false) {
	update = true;
	textRectH("Status", 100.f, 10.f, 48.f, NULL);

	textRectH("Name:", 30.f, 150.f, NULL, NULL);
	textRectH("Level:", 30.f, 200.f, NULL, NULL);
	textRectH("Current EXP:", 30.f, 250.f, NULL, NULL);
	textRectH("Level up EXP:", 30.f, 300.f, NULL, NULL);
	textRectH("Items:", 30.f, 350.f, NULL, NULL);
	textRectH("Spells:", 30.f, 400.f, NULL, NULL);
	textRectH("Floor:", 30.f, 450.f, NULL, NULL);
	textRectH("Gold:", 30.f, 500.f, NULL, NULL);
	textRectH("Stat points:", 30.f, 550.f, NULL, NULL);
	textRectH("Weapon:", 30.f, 600.f, NULL, NULL);
	textRectH("Armor:", 30.f, 650.f, NULL, NULL);

	textRectH("Effects:", 600.f, 150.f, NULL, NULL);

	// Stats
	// index starts at 13
	for (unsigned int i = 0; i < NUM_NON_CUR_STATS; i++) {
		textRectH(StatConst::FULL_STATS[i], 400.f, 150.f + i * 50.f, NULL, NULL);
	}

	// changing stats:
	// index starts at 19
	for (unsigned int i = 0; i < 11; i++) {
		textRectH("", 200.f, 150.f + i * 50.f, NULL, NULL);
	}

	// effects
	textRectH("", 720.f, 150.f, NULL, NULL);

    // stats
	// index starts at 31
	for (unsigned int i = 0; i < NUM_NON_CUR_STATS; i++)
		textRectH("", 550.f, 150.f + i * 50.f, NULL, NULL);

	textboxH("Reset Scroll", 620.f, 390.f, 180.f, 50.f, []() {
		viewEffect.reset(sf::FloatRect(0, 0, DEFAULT_SCREEN_X, DEFAULT_SCREEN_Y));
	});
	textboxH("^", 620.f, 190.f, 50.f, 50.f, []() {
		viewEffect.move(sf::Vector2f(0.f, 10.f));
	});
	textboxH("v", 620.f, 290.f, 50.f, 50.f, []() {
		viewEffect.move(sf::Vector2f(0.f, -10.f));
	});
}

bool Status_Screen::handleClickEvent() {
	if (mouseInButton(ExitButton)) {
		viewEffect.reset(sf::FloatRect(0, 0, 1200, 800));
		goToPrevScreen(StatusScreen);
		return true;
	}
	return false;
}

void Status_Screen::updateDraw() {
	// name
	texts[19].setString(Game_Manager::player.getName());
	// level
	texts[20].setString(std::to_string(Game_Manager::player.getLVL()));
	// current
	texts[21].setString(std::to_string(Game_Manager::player.getCurEXP()));
	// level up EXP
	texts[22].setString(std::to_string(Game_Manager::player.getLVLUpEXP()));
	// items
	texts[23].setString(std::format("{} / {} items",
		Game_Manager::items.size(),
		Game_Manager::player.getMaxItems()));
	// spells
	texts[24].setString(std::format("{} / {} spells",
		Game_Manager::spells.size(), Game_Manager::MAX_INV_SPELL_SLOTS));
	// floor
	texts[25].setString(std::to_string(Game_Manager::player.getFloor()));
	// gold
	texts[26].setString(std::to_string(Game_Manager::player.getGold()));
	// stat points
	texts[27].setString(std::to_string(Game_Manager::player.getStatPts()));
	// weapon
	texts[28].setString(Game_Manager::plWeapon->getName());
	// armor
	texts[29].setString(Game_Manager::plArmor->getName());
	// effects
	std::string effects_str = "";
	const std::vector<Effect> effects = Game_Manager::player.getEffect();
	for (const Effect& effect : Game_Manager::player.getEffect()) {
		effects_str.append(std::to_string(effect.stat_difference) +
			" to " +
			StatConst::FULL_STATS[effect.stat_changed % NUM_NON_CUR_STATS] +
			" for " +
			std::to_string(effect.change_turns) +
			" turns.\n");
	}
	effect_txt.setString(effects_str);
	texts[30].setString(std::to_string(effects.size()));
	// stats
	for (unsigned int i = 0; i < NUM_NON_CUR_STATS; i++)
		texts[i + 31].setString(std::to_string(Game_Manager::player.getStat((Stat)i)));
}

void Status_Screen::draw() {
	window.draw(map_rects["background"]);
	Screen::draw();

	window.setView(viewEffect);
	window.draw(effect_txt);
	window.setView(viewUI);
}

void Status_Screen::handleMouseEvent() {
	if (event.type == sf::Event::MouseWheelScrolled) {
		const float delta = event.mouseWheelScroll.delta;
		viewEffect.move(sf::Vector2f(0.f, delta * 5));
	}
}

void Status_Screen::changeTheme() {
	effect_txt.changeTheme();
}