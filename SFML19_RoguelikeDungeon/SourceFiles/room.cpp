/**
*
* File: room.cpp
* Description: Contain implementations for the Room class.
*
*/

#include "room.h"
#include "texture_manager.h"

Room::Room() {
	rm_draw.setFillColor(sf::Color::White);
	rm_draw.setOutlineThickness(3.f);

	sf::Color rgb(rand() % 50 + 100, rand() % 75, rand() % 75);

	rm_draw.setOutlineColor(rgb);

	int texture_num = rand() % Texture_Manager::num_rooms;
	rm_draw.setTexture(&Texture_Manager::tex_rooms[texture_num], false);

}

void Room::set_pos_and_size(int x, int y, int sx, int sy) {
	if (sx != -1 && sy != -1) {
		rm_draw.setSize(sf::Vector2f(sx, sy));
		rm_draw.setTextureRect(sf::IntRect(0, 0, rm_draw.getSize().x, rm_draw.getSize().y));
	}
	rm_draw.setPosition(x, y);
}

void Room::set_door(int x, int y, int purpose) {
	if (purpose > -1) {
		door_rotation = purpose;
		door = true;
		(purpose % 2 == 1) ? dr_draw.setSize(sf::Vector2f(0, 120)) : dr_draw.setSize(sf::Vector2f(120, 0));

		dr_draw.setFillColor(sf::Color::Black);
		dr_draw.setOutlineColor(sf::Color::White);
		dr_draw.setOutlineThickness(1.5f);
	}
	else if (purpose == -2)
		dr_draw.setSize(sf::Vector2f(x, y));

	if (purpose >= -1)
		dr_draw.setPosition(x, y);
}

int Room::get_rm(char z) {
	switch (z) {
	case 'x':
		return rm_draw.getPosition().x;
	case 'y':
		return rm_draw.getPosition().y;
	case 'w':
		return rm_draw.getSize().x;
	case 'h':
		return rm_draw.getSize().y;
	case '1':
		return rm_draw.getPosition().x + rm_draw.getSize().x;
	case '2':
		return rm_draw.getPosition().y + rm_draw.getSize().y;
	case '3':
		return rm_draw.getPosition().y + rm_draw.getSize().x + rm_draw.getSize().y;
	}
}

int Room::get_door(char z) {
	switch (z) {
	case 'x':
		return dr_draw.getPosition().x;
	case 'y':
		return dr_draw.getPosition().y;
	case 'w':
		return dr_draw.getSize().x;
	case 'h':
		return dr_draw.getSize().y;
	case 'r':
		return door_rotation;
	case '0':
		return doors[0];
	case '1':
		return doors[1];
	case '2':
		return doors[2];
	case '3':
		return doors[3];
	}
}

bool Room::in_room(int x, int y, int x2, int y2) {
	return (x >= rm_draw.getPosition().x && y >= rm_draw.getPosition().y &&
		x2 <= rm_draw.getPosition().x + rm_draw.getSize().x && y2 <= rm_draw.getPosition().y + rm_draw.getSize().y);
}

bool Room::door_exist() { return door; }

bool Room::touch_door(int x, int y, int x2, int y2) {
	// if door does not exist, return false.
	if (!door)
		return false;
	// if the door is horizontal
	else if (door_rotation % 2 == 0)
		return ((y == dr_draw.getPosition().y || y2 == dr_draw.getPosition().y) &&
			(x >= dr_draw.getPosition().x && x2 <= dr_draw.getPosition().x + 120));
	// if the door is vertical
	else
		return ((x == dr_draw.getPosition().x || x2 == dr_draw.getPosition().x) &&
			(y >= dr_draw.getPosition().y && y2 <= dr_draw.getPosition().y + 120));
}

bool Room::get_doors(unsigned int i) { return doors[i]; }

void Room::set_doors(unsigned int i, bool j) { doors[i] = j; }

void Room::draw(sf::RenderWindow& window, char d) { (d == 'r') ? window.draw(rm_draw) : window.draw(dr_draw); }