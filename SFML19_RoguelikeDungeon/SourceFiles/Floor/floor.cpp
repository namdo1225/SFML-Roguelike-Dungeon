/**
*
* File: floor.cpp
* Description: Contain implementations for Floor class.
*
*/

#include "env.h"
#include "Floor/floor.h"
#include "Manager/sf_manager.h"
#include "Tool/item.h"
#include <cfloat>
#include <cstdlib>
#include <Floor/collectible.h>
#include <Floor/gold_collectible.h>
#include <Floor/interactible.h>
#include <Floor/room.h>
#include <Floor/shop.h>
#include <Floor/stair.h>
#include <SFML/Graphics/Rect.hpp>
#include <State/player_state.h>
#include <utility>
#include <Floor/door.h>
#include "util.h"

const unsigned int Floor::RETRY_LIMITS;

Floor::Floor(bool load) {
	if (!load) {
		createRoomDoor();
		createStair();
		createShop();
		createCollectible();
		createGold();
		createInteractible();
	}
}

void Floor::createRoomDoor() {
	unsigned int maxRm{ rand() % 10 + 2 + (Player_State::player.getFloor() / 25) };
	unsigned int retries = 50;

	for (unsigned int i{ 0 }; i < maxRm; i++) {
		unsigned int unscaledH{ std::min(rand() % 10 + 10 + (Player_State::player.getFloor() / 15), Room::MAX_HEIGHT) };
		unsigned int unscaledW{ std::min(rand() % 10 + 10 + (Player_State::player.getFloor() / 15), Room::MAX_WIDTH) };
		float scaledW = unscaledW * SF_Manager::TILE;
		float scaledH = unscaledH * SF_Manager::TILE;

		if (i == 0) {
			rooms.push_back(Room(0, 0, unscaledW, unscaledH));
			continue;
		}
		else if (i % 3 == 0) {
			unsigned int newHW = rand() % 3 + 1;
			(i % 2 == 0) ? unscaledH = newHW : unscaledW = newHW;
			scaledW = unscaledW * SF_Manager::TILE;
			scaledH = unscaledH * SF_Manager::TILE;
		}

		unsigned int prevRoomId = rand() % rooms.size();
		Room& randRoom = rooms[prevRoomId];

		// Place room adjacent of random room
		// Meant for the new room. The opposite direction should be passed for existing room.
		Direction direction = (Direction)(rand() % 4);

		float x = randRoom.getRoom('1');
		float y = randRoom.getRoom('2');

		switch (direction) {
		case Top:
			x = nearestNonErrorTile(randRoom, false);
			break;
		case Bottom:
			x = nearestNonErrorTile(randRoom, false);
			y = randRoom.getRoom('y') - scaledH;
			break;
		case Left:
			y = nearestNonErrorTile(randRoom, true);
			break;
		case Right:
			x = randRoom.getRoom('x') - scaledW;
			y = nearestNonErrorTile(randRoom, true);
			break;
		}

		// If room is out of bound with float min/max / 2, skip.
		if (x + scaledW >= FLT_MAX / 2 || y + scaledH >= FLT_MAX / 2 ||
			x <= -FLT_MAX / 2 || y <= -FLT_MAX / 2) {
			continue;
		}

		// Check that new room doesn't intersects with other rooms
		bool intiateRetry = false;
		bool foundConflict = false;
		for (Room& rm : rooms)
			if (rm.intersects(sf::FloatRect(x, y, scaledW, scaledH))) {
				if (retries)
					intiateRetry = true;
				foundConflict = true;
				break;
			}

		if (foundConflict) {
			if (intiateRetry) {
				retries--;
				i--;
			}
			continue;
		}

		// Choose a door position using randomization
		// First choose the door's size.
		unsigned int tempUnscaledSize = (std::min(randRoom.getRoom('x') + randRoom.getRoom('w'), static_cast<int>(x + scaledW))
			- std::max(randRoom.getRoom('x'), static_cast<int>(x)));
		unsigned int possibleSize = tempUnscaledSize / SF_Manager::TILE;
		if (!possibleSize) {
			tempUnscaledSize = (std::min(randRoom.getRoom('y') + randRoom.getRoom('h'), static_cast<int>(y + scaledH))
				- std::max(randRoom.getRoom('y'), static_cast<int>(y)));
			possibleSize = tempUnscaledSize / SF_Manager::TILE;
		}

		unsigned int doorSize = rand() % std::min(possibleSize, 5U) + 1;

		// Assumes Top, Left direction first
		float doorX = x;
		float doorY = y;

		unsigned int doorOffset = possibleSize - doorSize;
		float finalDoorOffset = (doorOffset ? (rand() % (possibleSize - doorSize)) : 0) * SF_Manager::TILE;

		switch (direction) {
		case Top:
			doorX = finalDoorOffset + doorX;
			break;
		case Bottom:
			doorY = y + scaledH;
			doorX = finalDoorOffset + doorX;
			break;
		case Left:
			doorY = finalDoorOffset + doorY;
			break;
		case Right:
			doorX = x + scaledW;
			doorY = finalDoorOffset + doorY;
			break;
		}

		Room newRoom = Room(x, y, unscaledW, unscaledH);

		float scaledDoorSize = doorSize * SF_Manager::TILE;

		newRoom.addAdjacentRoom(Door(doorX, doorY, scaledDoorSize, direction, rooms.size(), prevRoomId), &randRoom);

		randRoom.addAdjacentRoom(Door(doorX, doorY, scaledDoorSize, (Direction)((direction + 2) % 4), prevRoomId, rooms.size()), &newRoom);

		rooms.push_back(newRoom);
	}
}

int Floor::calculateRandomPosition(Room& rm, bool useY) {
	int basePosition = useY ? rm.getRoom('y') : rm.getRoom('x');
	int size = useY ? rm.getRoom('h') : rm.getRoom('w');

	return basePosition + std::max(Util::nearestMultiple(rand() % size, SF_Manager::TILE) - SF_Manager::TILE, 0);
}

Room& Floor::getRoomByPosition(const sf::FloatRect& rect) {
	for (Room& rm : rooms)
		if (rm.intersects(rect))
			return rm;
}

sf::FloatRect Floor::calculateRect(Room& rm) {
	return sf::FloatRect(calculateRandomPosition(rm, false),
		calculateRandomPosition(rm, true), SF_Manager::TILE, SF_Manager::TILE);
}

void Floor::setRmPosSize(int rand_door, int& rand_coord, int& rand_size, int& rand_coord_2,
	int& rand_size_2, int rand_room_coord, int rand_room_size, int rand_room_coord_2, int rand_room_size_2, bool& conflict_room, int& k) {
	(k < 3) ? k += 1 : k = 0;

	if (rand_door == 0 || rand_door == 3)
		while (rand_coord_2 != rand_room_coord_2 + rand_room_size_2)
			rand_coord_2 = rand_room_coord_2 + rand_room_size_2, rand_size_2 = (rand() % 6 + 4 - k) * 120;
	else if (rand_door == 1 || rand_door == 2) {
		while (rand_coord_2 + rand_size_2 != rand_room_coord_2) {
			int temp_2 = rand_room_coord_2 / 120;
			int temp_3 = rand() % 12 + (temp_2 - 12);
			int temp_4 = temp_2 - temp_3;

			rand_coord_2 = temp_3 * 120, rand_size_2 = temp_4 * 120;
		}
	}

	while ((!(rand_coord + rand_size < rand_room_coord + rand_room_size && rand_room_coord < rand_coord && rand_coord + rand_size - 120 >= rand_room_coord) and
		!(rand_coord + rand_size > rand_room_coord + rand_room_size && rand_coord > rand_room_coord && rand_coord + 120 <= rand_room_coord + rand_room_size) and
		!(rand_coord >= rand_room_coord && rand_coord + rand_size <= rand_room_coord + rand_room_size) &&
		!(rand_coord <= rand_room_coord && rand_coord + rand_size >= rand_room_coord + rand_room_size)) || conflict_room) {

		rand_coord = (rand() % (rand_room_size / 120) + (rand_room_coord / 120)) * 120, rand_size = (rand() % 6 + 4 - k) * 120;
		conflict_room = false;
		(k < 3) ? k += 1 : k = 0;
	}
}

void Floor::setDoorPos1(int& rand_door_coord, int rand_coord, int rand_size) { rand_door_coord = rand_coord + rand_size; }

void Floor::setDoorPos2(int& rand_door_coord, int rand_coord, int rand_size, int rand_room_coord, int rand_room_size) {
	if (rand_coord + rand_size < rand_room_coord + rand_room_size && rand_coord < rand_room_coord && rand_coord + rand_size > rand_room_coord && rand_room_coord + 120 <= rand_coord + rand_size) {
		int tem_1{ rand_coord + rand_size - rand_room_coord };

		rand_door_coord = (rand() % (tem_1 / 120) + (rand_room_coord / 120)) * 120;

		if (rand_door_coord == rand_coord + rand_size)
			rand_door_coord -= 120;
	}
	else if (rand_coord + rand_size > rand_room_coord + rand_room_size && rand_coord > rand_room_coord && rand_coord < rand_room_coord + rand_room_size && rand_coord + 120 <= rand_room_coord + rand_room_size) {
		int tem_1{ rand_room_coord + rand_room_size - rand_coord };

		rand_door_coord = (rand() % (tem_1 / 120) + (rand_coord / 120)) * 120;

		if (rand_door_coord == rand_room_coord + rand_room_size)
			rand_door_coord -= 120;
	}
	else if (rand_coord >= rand_room_coord && rand_coord + rand_size <= rand_room_coord + rand_room_size)
		rand_door_coord = (rand() % (rand_size / 120) + (rand_coord / 120)) * 120;
	else if (rand_coord <= rand_room_coord && rand_coord + rand_size >= rand_room_coord + rand_room_size)
		rand_door_coord = (rand() % (rand_room_size / 120) + (rand_room_coord / 120)) * 120;
}

void Floor::createStair() {
	Room& rm = rooms[rand() % static_cast<int>(rooms.size())];
	stair = Stair(calculateRandomPosition(rm, false), calculateRandomPosition(rm, true));
}

void Floor::createShop() {
	if (!ALWAYS_SPAWN_SHOP && (rand() % 10) != 0) return;

	Room& rm = rooms[rand() % static_cast<int>(rooms.size())];

	int x = calculateRandomPosition(rm, false),
		y = calculateRandomPosition(rm, true);


	while (stair.intersects(shop.getGlobalBounds()))
		x = calculateRandomPosition(rm, false), y = calculateRandomPosition(rm, true);

	shop = Shop(x, y);
	shopExist = true;
}

int Floor::getShopPos(char z) { return z == 'x' ? shop.getPosition().x : shop.getPosition().y; }

int Floor::getStairPos(char z) { return z == 'x' ? stair.getPosition().x : stair.getPosition().y; }

bool Floor::isShopExist() { return shopExist; }

void Floor::draw(bool map) {
	for (Room& rm : rooms)
		if (!map || rm.getVisited())
			rm.draw(false);

	for (Room& rm : rooms)
		if (!map || rm.getVisited())
			rm.draw(true);

	if (map)
		return;

	SF_Manager::window.draw(stair);

	if (shopExist)
		SF_Manager::window.draw(shop);

	for (Gold_Collectible gold : golds)
		SF_Manager::window.draw(gold);

	for (Collectible col : collectibles)
		SF_Manager::window.draw(col);

	for (Interactible interactible : interactibles)
		if (!interactible.getHidden())
			SF_Manager::window.draw(interactible);
}

void Floor::setShopPos(int x, int y) { shop.setPosition(x, y); }

void Floor::setStairPos(int x, int y) { stair.setPosition(x, y); }

void Floor::loadRoom(int x, int y, int sx, int h, bool visited) {
	rooms.push_back(Room());
	rooms.back().setPosSize(x, y, sx, h);
	if (visited)
		rooms.back().setVisisted();
}

void Floor::loadDoor(unsigned int from, unsigned int to, int x, int y, int size, Direction direction) {
	rooms[from].addAdjacentRoom(Door(x, y, size, direction, from, to), &rooms[to]);
}

void Floor::loadStair(int x, int y) { stair = Stair(x, y); }

void Floor::loadCollectible(int x, int y, unsigned int id) { collectibles.push_back(Collectible(x, y, id)); }

void Floor::loadGold(int x, int y, unsigned int amount) { golds.push_back(Gold_Collectible(x, y, amount)); }

void Floor::loadShop(int x, int y) { shop = Shop(x, y); }

void Floor::loadInteractible(int x, int y, bool hidden) { interactibles.push_back(Interactible(x, y, hidden)); }

void Floor::createCollectible() {
	int itemLimit{ rand() % 6 + static_cast<int>(Player_State::player.getFloor() * 0.25) };
	unsigned int counter{ 0 };

	for (unsigned int i{ 0 }; i < itemLimit && counter < RETRY_LIMITS; i++) {
		Room& rm = rooms[rand() % static_cast<int>(rooms.size())];

		sf::FloatRect rect = calculateRect(rm);
		bool restartLoop = false;

		for (Collectible& col : collectibles)
			while (col.intersects(rect) && counter < RETRY_LIMITS && !restartLoop) {
				counter += 1;
				restartLoop = true;
			}

		if (restartLoop) {
			i--;
			continue;
		}

		if (counter >= RETRY_LIMITS)
			break;

		unsigned int item{ static_cast<unsigned int>(rand() % 3 + 1) };

		if (item > 2)
			item = Item::items.size() ? static_cast<unsigned int>(rand() % Item::items.size() + 1) : 0;
		collectibles.push_back(Collectible(rect.left, rect.top, item));
	}
}

void Floor::createGold() {
	int goldLimit{ rand() % 10 + static_cast<int>(Player_State::player.getFloor() * 0.50) };
	unsigned int counter{ 0 };

	for (unsigned int i{ 0 }; i < goldLimit && counter < RETRY_LIMITS; i++) {
		Room& rm = rooms[rand() % static_cast<int>(rooms.size())];

		sf::FloatRect rect = calculateRect(rm);
		bool restartLoop = false;

		for (Gold_Collectible& col : golds)
			while (col.intersects(rect) && counter < RETRY_LIMITS && !restartLoop) {
				counter += 1;
				restartLoop = true;
			}

		if (restartLoop) {
			i--;
			continue;
		}

		if (counter >= RETRY_LIMITS)
			break;

		golds.push_back(Gold_Collectible(rect.left, rect.top, 0));
	}
}

void Floor::createInteractible() {
	int interactibleLimit{ STARTING_INTERACTIBLES ? STARTING_INTERACTIBLES : rand()
		% 10 + static_cast<int>(Player_State::player.getFloor() * 0.50) };
	unsigned int counter{ 0 };

	for (unsigned int i{ 0 }; i < interactibleLimit && counter < RETRY_LIMITS; i++) {
		Room& rm = rooms[rand() % static_cast<int>(rooms.size())];

		sf::FloatRect rect = calculateRect(rm);
		bool restartLoop = false;

		for (Interactible& col : interactibles)
			while (col.intersects(rect) && counter < RETRY_LIMITS && !restartLoop) {
				counter += 1;
				restartLoop = true;
			}

		if (restartLoop) {
			i--;
			continue;
		}

		if (counter >= RETRY_LIMITS)
			break;

		int hidden{ rand() % 5 };
		interactibles.push_back(Interactible(rect.left, rect.top, !hidden));
	}
}

int Floor::nearestNonErrorTile(Room& rm, bool useY) {
	int position = rm.getRoom(useY ? 'y' : 'x');
	int size = rm.getRoom(useY ? 'h' : 'w');
	int finalRand = size - SF_Manager::TILE;

	return Util::nearestTile(finalRand ? rand() % finalRand + position : position);
}

bool Floor::intersectStair(const sf::FloatRect& rect) {
	return stair.getGlobalBounds().intersects(rect);
}

bool Floor::intersectShop(const sf::FloatRect& rect) {
	return shop.getGlobalBounds().intersects(rect);
}
