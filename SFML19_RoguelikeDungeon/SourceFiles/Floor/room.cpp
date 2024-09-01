/**
*
* File: room.cpp
* Description: Contain implementations for the Room class.
*
*/

#include "Floor/room.h"
#include "Manager/sf_manager.h"
#include "Manager/texture_manager.h"
#include <cstdlib>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

Room::Room() {
	setFillColor(sf::Color::White);
	setOutlineThickness(3.f);

	sf::Color rgb(rand() % 50 + 100, rand() % 75, rand() % 75);

	setOutlineColor(rgb);

	int size = Texture_Manager::tex_rooms.size();

	if (size) {
		int texture_num = rand() % size;
		setTexture(&Texture_Manager::tex_rooms[texture_num], false);
	}

}

void Room::setPosSize(int x, int y, int sx, int sy) {
	if (sx != -1 && sy != -1) {
		setSize(sf::Vector2f(sx, sy));
		setTextureRect(sf::IntRect(0, 0, getSize().x, getSize().y));
	}
	setPosition(x, y);
}

void Room::setDoor(int x, int y, int rotation) {
	if (rotation > -1) {
		doorRotation = (Door)rotation;
		door = true;
		(rotation % 2 == 1) ? doorRect.setSize(sf::Vector2f(0, 120)) : doorRect.setSize(sf::Vector2f(120, 0));

		doorRect.setFillColor(sf::Color::Black);
		doorRect.setOutlineColor(sf::Color::White);
		doorRect.setOutlineThickness(1.5f);
	}
	else if (rotation == -2)
		doorRect.setSize(sf::Vector2f(x, y));

	if (rotation >= -1)
		doorRect.setPosition(x, y);
}

int Room::getRoom(char z) {
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

int Room::getDoor(char z) {
	switch (z) {
	case 'x':
		return doorRect.getPosition().x;
	case 'y':
		return doorRect.getPosition().y;
	case 'w':
		return doorRect.getSize().x;
	case 'h':
		return doorRect.getSize().y;
	case 'r':
		return doorRotation;
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

bool Room::inRoom(int x, int y, int x2, int y2) {
	return (x >= getPosition().x && y >= getPosition().y &&
		x2 <= getPosition().x + getSize().x && y2 <= getPosition().y + getSize().y);
}

bool Room::existDoor() { return door; }

bool Room::touchDoor(int x, int y, int x2, int y2) {
	// if door does not exist, return false.
	if (!door)
		return false;
	// if the door is horizontal
	else if (doorRotation % 2 == 0)
		return ((y == doorRect.getPosition().y || y2 == doorRect.getPosition().y) &&
			(x >= doorRect.getPosition().x && x2 <= doorRect.getPosition().x + 120));
	// if the door is vertical
	else
		return ((x == doorRect.getPosition().x || x2 == doorRect.getPosition().x) &&
			(y >= doorRect.getPosition().y && y2 <= doorRect.getPosition().y + 120));
}

bool Room::getDoors(unsigned int i) { return doors[i]; }

void Room::setDoors(unsigned int i, bool j) { doors[i] = j; }

void Room::draw(bool door) { door ? SF_Manager::window.draw(doorRect) : SF_Manager::window.draw(*this); }

bool Room::intersects(const sf::FloatRect& rect) {
	return getGlobalBounds().intersects(rect);
}
