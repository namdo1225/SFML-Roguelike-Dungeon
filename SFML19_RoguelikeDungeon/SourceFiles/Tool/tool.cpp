#include "Tool/tool.h"
#include "Manager/font_manager.h"
#include "Manager/game_manager.h"
#include "Manager/setting_manager.h"

const char* Tool::STAT_STRINGS[] = { "HP", "MP", "STR", "MGK", "DEF", "RES", "HP", "MP" };

Tool::Tool() {
	desc.setFillColor(sf::Color::White);
	desc.setCharacterSize(24);
	desc.setPosition(760, 150);

	icon.setFillColor(sf::Color::White);
	icon.setStyle(sf::Text::Bold);
	icon.setCharacterSize(38);

	txt.setOutlineColor(sf::Color::White);
	txt.setFillColor(sf::Color::White);
	txt.setPosition(825, 310);
	txt.setCharacterSize(38);

	icon.setFont(Font_Manager::get_selected());
	txt.setFont(Font_Manager::get_selected());
	desc.setFont(Font_Manager::get_selected());

	change_theme();
}

void Tool::draw(char type) {
	switch (type) {
	case 't':
		Game_Manager::window.draw(invisible_rect);
		Game_Manager::window.draw(icon);
		break;
	case 'd':
		Game_Manager::window.draw(desc);
		break;
	default:
		Game_Manager::window.draw(txt);
		break;
	}
}

int Tool::get_pos(char z) {
	return z == 'x' ? invisible_rect.getPosition().x : invisible_rect.getPosition().y;;
}

unsigned int Tool::get_sell_gd() {
	return sell_gd;
}

unsigned int Tool::get_buy_gd() {
	return buy_gd;
}

unsigned int Tool::get_id() {
	return id;
}

void Tool::set_pos(int x, int y) {
	icon.setPosition(x, y);
	invisible_rect.setPosition(x, y);
}

std::string Tool::get_letter() {
	return icon.getString();
}

bool Tool::contains(float x, float y) {
	return invisible_rect.getGlobalBounds().contains(sf::Vector2f(x, y));
}

void Tool::change_theme() {
	icon.flip_theme(Setting_Manager::light, Setting_Manager::theme);
	desc.flip_theme(Setting_Manager::light, Setting_Manager::theme);
	txt.flip_theme(Setting_Manager::light, Setting_Manager::theme);

	icon.setFont(Font_Manager::get_selected());
	desc.setFont(Font_Manager::get_selected());
	txt.setFont(Font_Manager::get_selected());
}

unsigned int Tool::get_range() {
	return range;
}

unsigned int Tool::get_type() {
	return type;
}
