/**
*
* File: room.cpp
* Description: Contain implementations for the Room class.
*
*/

#include "Floor/room.h"
#include "Manager/texture_manager.h"
#include "Manager/sf_manager.h"

Room::Room() {
	setFillColor(sf::Color::White);
	setOutlineThickness(3.f);

	sf::Color rgb(rand() % 50 + 100, rand() % 75, rand() % 75);

	setOutlineColor(rgb);

	int texture_num = rand() % Texture_Manager::num_rooms;
	setTexture(&Texture_Manager::tex_rooms[texture_num], false);

}

void Room::set_pos_and_size(int x, int y, int sx, int sy) {
	if (sx != -1 && sy != -1) {
		setSize(sf::Vector2f(sx, sy));
		setTextureRect(sf::IntRect(0, 0, getSize().x, getSize().y));
	}
	setPosition(x, y);
}

void Room::set_door(int x, int y, int purpose) {
	if (purpose > -1) {
		door_rotation = (Door)purpose;
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
	float x = getPosition().x;
	float y = getPosition().y;

	float sx = getSize().x;
	float sy = getSize().y;

	switch (z) {
	case 'x':
		return x;
	case 'y':
		return y;
	case 'w':
		return sx;
	case 'h':
		return sy;
	case '1':
		return x + sx;
	case '2':
		return y + sy;
	case '3':
		return y + sx + sy;
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
	case Top:
		return doors[Top];
	case Right:
		return doors[Right];
	case Bottom:
		return doors[Bottom];
	case Left:
		return doors[Left];
	}
}

bool Room::in_room(int x, int y, int x2, int y2) {
	return (x >= getPosition().x && y >= getPosition().y &&
		x2 <= getPosition().x + getSize().x && y2 <= getPosition().y + getSize().y);
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

void Room::draw(char d) { (d == 'r') ? SF_Manager::window.draw(*this) : SF_Manager::window.draw(dr_draw); }

bool Room::intersects(const sf::FloatRect& rect) {
	return getGlobalBounds().intersects(rect);
}
