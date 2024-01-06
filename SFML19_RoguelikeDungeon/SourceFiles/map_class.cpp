/**
*
* File: map_class.cpp
* Description: Contain implementations for the Stair class.
*
*/

#include "map_class.h"

std::array<sf::Text, 4> Map::directions = { sf::Text(), sf::Text(), sf::Text(), sf::Text() };
std::array<sf::Text, 2> Map::zooms = { sf::Text(), sf::Text() };
int Map::factor = 2;

Map::Map() {
}

Map::Map(std::vector<Room> &rooms) {
	Room first = rooms[0];
	int offset_x = first.get_rm('x') / 8 - 550;
	int offset_y = first.get_rm('y') / 8 - 350;

	for (Room rm : rooms) {
		rm.set_pos_and_size(rm.get_rm('x') / 8 - offset_x, rm.get_rm('y') / 8 - offset_y, rm.get_rm('w') / 8, rm.get_rm('h') / 8);

		if (rm.door_exist())
			rm.set_door(rm.get_door('x') / 8 - offset_x, rm.get_door('y') / 8 - offset_y, -1),
			rm.set_door((rm.get_door('r') % 2 == 0) ? 15 : 0, (rm.get_door('r') % 2 == 0) ? 0 : 15, -2);

		map.push_back(rm);
	}
}

void Map::draw(sf::RenderWindow& window) {
	for (Room rm : map)
		rm.draw(window, 'r');

	for (Room rm : map)
		rm.draw(window, 'd');

	for (sf::Text txt : directions)
		window.draw(txt);

	for (sf::Text txt : zooms)
		window.draw(txt);
}

void Map::setup(sf::Font& font) {
	directions[0].setString("up");
	directions[1].setString("ri");
	directions[2].setString("do");
	directions[3].setString("le");

	directions[0].setPosition(1050, 500);
	directions[1].setPosition(1120, 550);
	directions[2].setPosition(1050, 600);
	directions[3].setPosition(1000, 550);

	zooms[0].setString("+");
	zooms[1].setString("-");

	zooms[0].setPosition(1000, 425);
	zooms[1].setPosition(1120, 425);

	for (sf::Text& txt : directions) {
		txt.setCharacterSize(48);
		txt.setStyle(sf::Text::Bold);
		txt.setFillColor(sf::Color::White);
		txt.setOutlineThickness(3.f);
		txt.setOutlineColor(sf::Color::Black);
		txt.setFont(font);
	}

	for (sf::Text& txt : zooms) {
		txt.setCharacterSize(48);
		txt.setStyle(sf::Text::Bold);
		txt.setFillColor(sf::Color::White);
		txt.setOutlineThickness(3.f);
		txt.setOutlineColor(sf::Color::Black);
		txt.setFont(font);
	}
}

void Map::move(char direction) {
	int moveV = 0;
	int moveH = 0;

	if (direction == 'u')
		moveV = -10;
	else if (direction == 'd')
		moveV = 10;

	if (direction == 'l')
		moveH = -10;
	else if (direction == 'r')
		moveH = 10;

	for (unsigned int i = 0; i < map.size(); i++) {
		map[i].set_pos_and_size(map[i].get_rm('x') + moveH,
			map[i].get_rm('y') + moveV,
			map[i].get_rm('w'),
			map[i].get_rm('h'));

		if (map[i].door_exist())
			map[i].set_door(map[i].get_door('x') + moveH, map[i].get_door('y') + moveV, -1),
			map[i].set_door((map[i].get_door('r') % 2 == 0) ? 15 : 0, (map[i].get_door('r') % 2 == 0) ? 0 : 15, -2);

	}
}

void Map::zoom(char zoom) {
	if (zoom != 'i') {
		for (unsigned int i = 0; i < map.size(); i++)
			if (map[i].get_rm('w') < 50 || map[i].get_rm('h') < 50)
				return;
	}
	else
		for (unsigned int i = 0; i < map.size(); i++)
			if (map[i].get_rm('w') > 10000 || map[i].get_rm('h') > 10000)
				return;

	bool check = zoom == 'i';
	int offset_x = (check ? map[0].get_rm('x') * factor : map[0].get_rm('x') / factor) - 550;
	int offset_y = (check ? map[0].get_rm('y') * factor : map[0].get_rm('y') / factor) - 350;

	for (unsigned int i = 0; i < map.size(); i++) {
		map[i].set_pos_and_size((check ? map[i].get_rm('x') * factor : map[i].get_rm('x') / factor) - offset_x,
			(check ? map[i].get_rm('y') * factor : map[i].get_rm('y') / factor) - offset_y,
			(check ? map[i].get_rm('w') * factor : map[i].get_rm('w') / factor),
			(check ? map[i].get_rm('h') * factor : map[i].get_rm('h') / factor));

		if (map[i].door_exist())
			map[i].set_door((check ? map[i].get_door('x') * factor : map[i].get_door('x') / factor) - offset_x,
				(check ? map[i].get_door('y') * factor : map[i].get_door('y') / factor) - offset_y, -1),
			map[i].set_door((map[i].get_door('r') % 2 == 0) ? 15 : 0,
				(map[i].get_door('r') % 2 == 0) ? 0 : 15, -2);

	}
}

void Map::handleEvent(int x, int y) {
	if (x > 1050 && x < 1100 && y > 500 && y < 550)
		this->move('u');
	else if (x > 1050 && x < 1100 && y > 600 && y < 650)
		this->move('d');
	else if (x > 1120 && x < 1170 && y > 550 && y < 600)
		this->move('r');
	else if (x > 1000 && x < 1050 && y > 550 && y < 600)
		this->move('l');
	else if (x > 1000 && x < 1050 && y > 425 && y < 475)
		this->zoom('i');
	else if (x > 1120 && x < 1170 && y > 425 && y < 475)
		this->zoom('o');
}