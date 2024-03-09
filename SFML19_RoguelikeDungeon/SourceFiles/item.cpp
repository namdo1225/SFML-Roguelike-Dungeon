/**
*
* File: item.cpp
* Description: Contains implementations of Item class and its sub-classes.
*
*/

#include "item.h"

Item::Item() {
	icon.setPosition(-100, -100);
	txt.setPosition(-100, -100);

	txt.setOutlineColor(sf::Color::White);
	txt.setFillColor(sf::Color::White);
	txt.setPosition(825, 310);
	txt.setCharacterSize(48);

	description.setCharacterSize(24);
	description.setPosition(760, 150);

	icon.setStyle(sf::Text::Bold);
	icon.setCharacterSize(42);
}

Item::Item(int t_id, int t_type, bool t_modify_act,
	bool t_hp_mp_other, Stat t_stat_type, int t_quantity,
	int t_range, int t_buy, int t_sell, const char abbre[3],
	sf::Font& font) : Item() {
	id = t_id;
	type = t_type;
	modify_or_act = t_modify_act;
	hp_mp_other = t_hp_mp_other;
	stat = t_stat_type;
	quantity = t_quantity;
	range = t_range;
	buy_gd = t_buy;
	sell_gd = t_sell;

	icon.setString(abbre);
	txt.setString(abbre);

	icon.setFont(font);
	txt.setFont(font);
	description.setFont(font);
}

int Item::get_pos(char z) { return z == 'x' ? icon.getPosition().x : icon.getPosition().y; }

void Item::set_pos(int x, int y) { icon.setPosition(x, y); }

unsigned int Item::get_id() { return id; }

unsigned int Item::get_sell_gd() { return sell_gd; }

unsigned int Item::get_buy_gd() { return buy_gd; }

unsigned int Item::get_type() { return type; }

Item* Item::create_itm(unsigned int id, sf::Font& font) {
	switch (id) {
	case 1:
		return new Health_Potion(font);
	case 2:
		return new Mana_Potion(font);
	case 3:
		return new Wooden_Staff(font);
	case 4:
		return new Iron_Armor(font);
	case 5:
		return new Magic_Armor(font);
	case 6:
		return new Iron_Sword(font);
	}
}

std::string Item::get_letter() { return icon.getString(); }

unsigned int Item::get_range() { return range; }

Stat Item::get_stat() { return stat; }

int Item::get_quantity() { return quantity; }

bool Item::get_hp_mp_other() { return hp_mp_other; }

void Item::use(Player &player) {}

void Item::draw(sf::RenderWindow& window, char type) {
	switch (type) {
	case 't':
		window.draw(icon);
		break;
	case 'd':
		window.draw(description);
		break;
	default:
		window.draw(txt);
		break;
	}	
}

Place_Holder::Place_Holder() {
	id = 0;
	type = 3;
}

Health_Potion::Health_Potion(sf::Font& font) : Item(1, 0, true, false, Max_Hp, 5, 0, 15, 3, "HI", font) {
	description.setString("Health Potion\n\nA potion that restore a \nsmall amount of health.\n\n+5 HP\n\nBUY: 15G\nSELL: 3G");
}

void Health_Potion::use(Player& player) {
	unsigned int difference = player.get_stat(Max_Hp) - player.get_stat(Hp);
	player.set_stat(Hp, difference > 5 ? player.get_stat(Hp) + 5 : player.get_stat(Max_Hp));
}

Iron_Sword::Iron_Sword(sf::Font &font) : Item(6, 1, true, true, Str, 3, 1, 40, 10, "IS", font) {
	description.setString("Iron Sword\n\nA common sword.\n\n+3 STR\n\nBUY: 40G\nSELL: 10G");
}

Iron_Armor::Iron_Armor(sf::Font& font) : Item(4, 2, true, true, Def, 3, 0, 40, 10, "IA", font) {
	description.setString("Iron Armor\n\nA common armor.\n\n+3 DEF\n\nBUY: 40G\nSELL: 10G");
}

Magic_Armor::Magic_Armor(sf::Font& font) : Item(5, 2, true, true, Res, 2, 0, 50, 20, "MA", font) {
	description.setString("Magic Armor\n\nA common magic armor.\n\n+2 RES\n\nBUY: 50G\nSELL: 20G");
}

Mana_Potion::Mana_Potion(sf::Font& font) : Item(2, 0, true, false, Max_Hp, 5, 0, 15, 3, "MI", font) {
	description.setString("Mana Potion\n\nA potion that restore a \nsmall amount of mana.\n\n+5 MP\n\nBUY: 15G\nSELL: 3G");
}

void Mana_Potion::use(Player& player) {
	unsigned int difference = player.get_stat(Max_Mp) - player.get_stat(Mp);
	player.set_stat(Mp, difference > 5 ? player.get_stat(Mp) + 5 : player.get_stat(Max_Mp));
}

Wooden_Staff::Wooden_Staff(sf::Font& font) : Item(3, 1, true, true, Mgk, 2, 2, 50, 20, "WS", font) {
	description.setString("Wooden Staff\n\nA common staff allowing the use\nof magical attacks.\n\n+2 MAG\n\nBUY: 50G\nSELL: 20G");
}