/**
*
* File: room.cpp
* Description: Contain implementations for the Room class.
*
*/

#include "Floor/room.h"
#include "Manager/sf_manager.h"
#include "Manager/texture_manager.h"
#include <array>
#include <cstdlib>
#include <Floor/door.h>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>

const unsigned int Room::MAX_ROOMS;
const unsigned int Room::MAX_WIDTH;
const unsigned int Room::MAX_HEIGHT;

Room::Room(float x, float y, int unscaledW, int unscaledH) {
	//setFillColor(sf::Color::White);
	//setOutlineThickness(2.f);
	sf::Color rgb(rand() % 150 + 50, rand() % 150 + 50, rand() % 150 + 50, 100);
	setFillColor(rgb);

	int size = Texture_Manager::tex_rooms.size();

	if (size) {
		int texture_num = rand() % size;
		setTexture(&Texture_Manager::tex_rooms[texture_num], false);
	}

	if (unscaledW != -1 && unscaledH != -1 && x != -1 && y != -1) {
		setPosition(x, y);
		setSize(sf::Vector2f(unscaledW * SF_Manager::TILE, unscaledH * SF_Manager::TILE));
		setTextureRect(sf::IntRect(0, 0, getSize().x, getSize().y));
	}
}

void Room::setPosSize(int x, int y, int sx, int h) {
	if (sx != -1 && h != -1) {
		setSize(sf::Vector2f(sx, h));
		setTextureRect(sf::IntRect(0, 0, getSize().x, getSize().y));
	}
	setPosition(x, y);
}

int Room::getRoom(char z) {
	float x = getPosition().x;
	float y = getPosition().y;

	float w = getSize().x;
	float h = getSize().y;

	switch (z) {
	case 'x':
		return x;
	case 'y':
		return y;
	case 'w':
		return w;
	case 'h':
		return h;
	case '1':
		return x + w;
	case '2':
		return y + h;
	case '3':
		return y + w + h;
	}
}

bool Room::inRoom(int x, int y, int x2, int y2) {
	return (x >= getPosition().x && y >= getPosition().y &&
		x2 <= getPosition().x + getSize().x && y2 <= getPosition().y + getSize().y);
}

std::array<std::vector<Door>, 4> Room::getDoors() { return doors; }

void Room::draw(bool drawDoor) {
	if (drawDoor)
		for (std::vector<Door>& doorDirection : doors)
			for (Door& door : doorDirection)
				SF_Manager::window.draw(door);
	else
		SF_Manager::window.draw(*this);
}

bool Room::getVisited() {
	return visited;
}

void Room::setVisisted() {
	visited = true;
}

void Room::addAdjacentRoom(Door door, Room* room) {
	doors[door.getDirection()].push_back(door);
	connectedRooms[door.getDirection()].push_back(room);
}

bool Room::areEntitiesInDoorRange(Room* otherRoom, const sf::FloatRect& entity1, const sf::FloatRect& entity2) {
	for (unsigned int i = 0; i < connectedRooms.size(); i++)
		for (unsigned int j = 0; j < connectedRooms[i].size(); j++)
			if (doors[i][j].intersects(otherRoom->getGlobalBounds()))
				return doors[i][j].canPass(entity1.left, entity1.top) && doors[i][j].canPass(entity2.left, entity2.top);
	return false;
}

bool Room::touchDoor(Direction direction, const sf::FloatRect& entity) {
	for (Door& dr : doors[direction])
		if (dr.canPass(entity.left, entity.top, true))
			return true;

	for (Door& dr : doors[(direction + 2) % 4])
		if (dr.canPass(entity.left, entity.top, true))
			return true;

	return false;
}
