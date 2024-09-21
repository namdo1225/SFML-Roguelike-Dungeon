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
#include <array>
#include <cstdlib>
#include <Floor/collectible.h>
#include <Floor/gold_collectible.h>
#include <Floor/interactible.h>
#include <Floor/room.h>
#include <Floor/shop.h>
#include <Floor/stair.h>
#include <SFML/Graphics/Rect.hpp>

Floor::Floor(bool load) {
	if (!load) {
		createRoomDoor();
		createStair();
		createShop();
	}
}

void Floor::createRoomDoor() {
	// maximum floor size: 2160x2160, one square: 40x40
	int max_rm{ rand() % 10 + 2 };
	for (unsigned int i{ 0 }; i < max_rm; i++) {
		int sx{ (rand() % 6 + 4) * 120 }, sy{ (rand() % 6 + 4) * 120 },
			x{ (rand() % 26 + 36) * 120 }, y{ (rand() % 26 + 36) * 120 };

		while (!(x + sx <= 8640 && y + sy <= 8640 && x >= 4320 && y >= 4320))
			x = (rand() % 26 + 36) * 120, y = (rand() % 26 + 36) * 120,
			sx = (rand() % 6 + 4) * 120, sy = (rand() % 6 + 4) * 120;

		rooms.push_back(Room());

		if (i == 0) {
			sx += 480, sy += 480;
			rooms[i].setPosSize(x, y, sx, sy);
			continue;
		}

		Door rand_dr{ (Door)(rand() % 4) };
		int rand_rm{ rand() % (static_cast<int>(rooms.size()) - 1) };

		// make sure room has doors available. Otherwise, switch room.
		while (rooms[rand_rm].getDoors(0) == 1 && rooms[rand_rm].getDoors(1) == 1 and
			rooms[rand_rm].getDoors(2) == 1 && rooms[rand_rm].getDoors(3) == 1)
			rand_rm = rand() % (rooms.size() - 1);

		// make sure new room's door has the old room's door available. Otherwise, switch door.
		std::array<unsigned int, 4> door = { 2, 3, 0, 1 };
		bool choose_door = true;
		while (choose_door) {
			rand_dr = (Door)(rand() % 4);
			if (rooms[rand_rm].getDoors(door[rand_dr]) == 0) {
				rooms[rand_rm].setDoors(door[rand_dr], 1);
				choose_door = false;
			}
		}
		rooms[i].setDoors(rand_dr, 1);

		// set new room's position && size.
		int break_loop_index{ static_cast<int>(rooms.size()) - 1 }, break_loop_counter{ 0 }, remove{ 0 }, k{ 0 };
		bool conflict_room{ false };

		while (true) {
			if (remove >= 20) break;

			break_loop_counter = 0;
			if (rand_dr % 2 == 0)
				setRmPosSize(rand_dr, x, sx, y, sy, rooms[rand_rm].getRoom('x'), rooms[rand_rm].getRoom('w'), rooms[rand_rm].getRoom('y'), rooms[rand_rm].getRoom('h'), conflict_room, k);
			else if (rand_dr % 2 == 1)
				setRmPosSize(rand_dr, y, sy, x, sx, rooms[rand_rm].getRoom('y'), rooms[rand_rm].getRoom('h'), rooms[rand_rm].getRoom('x'), rooms[rand_rm].getRoom('w'), conflict_room, k);

			for (unsigned int j{ 0 }; j < rooms.size() - 1; j++) {
				if (x >= rooms[j].getRoom('1') || x + sx <= rooms[j].getRoom('x') or
					y >= rooms[j].getRoom('2') || y + sy <= rooms[j].getRoom('y'))
					break_loop_counter += 1;
				else
					conflict_room = true;
			}

			if (break_loop_counter == break_loop_index) break;
			remove++;
		}

		if (remove >= 20) {
			rooms.pop_back();
			max_rm--;
			i--;
			continue;
		}

		rooms[i].setPosSize(x, y, sx, sy);

		// set door's x or y
		int rand_dr_x{ (rand() % (sx / 40) + (x / 40)) * 40 }, rand_dr_y{ (rand() % (sy / 40) + (y / 40)) * 40 };

		switch (rand_dr) {
		case 0:
			setDoorPos1(rand_dr_y, y, 0); break;
		case 2:
			setDoorPos1(rand_dr_y, y, sy); break;
		case 1:
			setDoorPos1(rand_dr_x, x, sx); break;
		case 3:
			setDoorPos1(rand_dr_x, x, 0); break;
		}

		(rand_dr % 2 == 0) ? setDoorPos2(rand_dr_x, x, sx, rooms[rand_rm].getRoom('x'), rooms[rand_rm].getRoom('w')) :
			setDoorPos2(rand_dr_y, y, sy, rooms[rand_rm].getRoom('y'), rooms[rand_rm].getRoom('h'));

		rooms[i].setDoor(rand_dr_x, rand_dr_y, rand_dr);
	}
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
	int rand_room{ rand() % static_cast<int>(rooms.size()) };

	int rx = rooms[rand_room].getRoom('x');
	int ry = rooms[rand_room].getRoom('y');
	int rw = rooms[rand_room].getRoom('w') - 40;
	int rh = rooms[rand_room].getRoom('h') - 40;

	int x{ (rand() % rh) + rx };
	int y{ (rand() % rw) + ry };

	while (!rooms[rand_room].inRoom(x, y, x + 40, y + 40)) {
		x = (rand() % rh) + rx;
		y = (rand() % rw) + ry;
	}

	stair = Stair(x, y);
}

void Floor::createShop() {
	if (!ALWAYS_SPAWN_SHOP && (rand() % 10) != 0) return;

	int rand_room{ rand() % static_cast<int>(rooms.size()) };

	int rx = rooms[rand_room].getRoom('x');
	int ry = rooms[rand_room].getRoom('y');
	int rw = rooms[rand_room].getRoom('w') - 40;
	int rh = rooms[rand_room].getRoom('h') - 40;

	int x = (rand() % rw) + rx;
	int y = (rand() % rh) + ry;
	float sx = stair.getPosition().x;
	float sy = stair.getPosition().y;

	bool notInRoom = !rooms[rand_room].inRoom(x, y, x + 40, y + 40);

	while (stair.intersects(shop.getGlobalBounds()) || notInRoom) {
		x = (rand() % rw) + rx;
		y = (rand() % rh) + ry;
	}

	shop = Shop(x, y);
	shopExist = true;
}

int Floor::getShopPos(char z) { return z == 'x' ? shop.getPosition().x : shop.getPosition().y; }

int Floor::getStairPos(char z) { return z == 'x' ? stair.getPosition().x : stair.getPosition().y; }

bool Floor::isShopExist() { return shopExist; }

void Floor::draw(bool map) {
	for (Room rm : rooms)
		if (!map || rm.getVisited())
			rm.draw();

	for (Room rm : rooms)
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

void Floor::loadRoom(int x, int y, int sx, int sy, bool visited) {
	rooms.push_back(Room());
	rooms.back().setPosSize(x, y, sx, sy);
	if (visited)
		rooms.back().setVisisted();
}

void Floor::loadDoor(int x, int y, int rot, int slot0, int slot1, int slot2, int slot3) { 
	rooms.back().setDoor(x, y, rot); 
	rooms.back().setDoors(0, slot0);
	rooms.back().setDoors(1, slot1);
	rooms.back().setDoors(2, slot2);
	rooms.back().setDoors(3, slot3);
}

void Floor::loadStair(int x, int y) { stair = Stair(x, y); }

void Floor::loadCollectible(int x, int y, unsigned int id) { collectibles.push_back(Collectible(x, y, id)); }

void Floor::loadGold(int x, int y, unsigned int amount) { golds.push_back(Gold_Collectible(0, amount, x, y)); }

void Floor::loadShop(int x, int y) { shop = Shop(x, y); }

void Floor::loadInteractible(int x, int y, bool hidden) { interactibles.push_back(Interactible(x, y, hidden)); }

void Floor::makeCollectible(unsigned int floor) {
	int rand_items{ rand() % 6 + static_cast<int>(floor * 0.25) };

	for (unsigned int i{ 0 }; i < rand_items; i++) {
		int rand_room{ rand() % static_cast<int>(rooms.size()) }, counter{ 0 },
			temp_x{ (rand() % (rooms[rand_room].getRoom('w') - 40)) + rooms[rand_room].getRoom('x') },
			temp_y{ (rand() % (rooms[rand_room].getRoom('h') - 40)) + rooms[rand_room].getRoom('y') };

		bool loop{ true };
		while (loop) {
			for (Collectible col : collectibles) {
				float tmp_x{ col.getPosition().x }, tmp_y{ col.getPosition().y };

				while (temp_x == tmp_x && temp_y == tmp_y || temp_x == -1 || temp_y == -1)
					temp_x = (rand() % (rooms[rand_room].getRoom('w') - 40)) + rooms[rand_room].getRoom('x'),
					temp_y = (rand() % (rooms[rand_room].getRoom('h') - 40)) + rooms[rand_room].getRoom('y');
				counter += 1;
			}

			if (collectibles.size() == counter) {
				unsigned int item{ static_cast<unsigned int>(rand() % 3 + 1) };

				if (item > 2)
					item = Item::items.size() ? static_cast<unsigned int>(rand() % Item::items.size() + 1) : 0;
				loop = false;
				collectibles.push_back(Collectible(temp_x, temp_y, item));
			}
		}
	}
}

void Floor::makeGold(unsigned int floor) {
	int rand_gold{ rand() % 10 + static_cast<int>(floor * 0.50) };

	for (unsigned int i{ 0 }; i < rand_gold; i++) {
		int rand_room{ rand() % static_cast<int>(rooms.size()) }, counter{ 0 },
			temp_x{ (rand() % (rooms[rand_room].getRoom('w') - 40)) + rooms[rand_room].getRoom('x') },
			temp_y{ (rand() % (rooms[rand_room].getRoom('h') - 40)) + rooms[rand_room].getRoom('y') };

		bool break_loop{ true };
		while (break_loop) {
			for (unsigned int j{ 0 }; j < golds.size(); j++) {
				float tmp_x{ golds[j].getPosition().x }, tmp_y{ golds[j].getPosition().y };

				while ((temp_x == tmp_x && temp_y == tmp_y) || temp_x == -1 || temp_y == -1) 
					temp_x = (rand() % (rooms[rand_room].getRoom('w') - 40)) + rooms[rand_room].getRoom('x'),
					temp_y = (rand() % (rooms[rand_room].getRoom('h') - 40)) + rooms[rand_room].getRoom('y');
				counter += 1;
			}

			if (golds.size() == counter) {
				break_loop = false;
				golds.push_back(Gold_Collectible(floor, 0, temp_x, temp_y));
			}
		}
	}
}

void Floor::makeInteractible(unsigned int floor) {
	int rand_interact{ STARTING_INTERACTIBLES ? STARTING_INTERACTIBLES : rand() % 50 + static_cast<int>(floor * 0.50) };

	for (unsigned int i{ 0 }; i < rand_interact; i++) {
		int rand_room{ rand() % static_cast<int>(rooms.size()) }, counter{ 0 },
		temp_x{ (rand() % (rooms[rand_room].getRoom('w') - 40)) + rooms[rand_room].getRoom('x') },
		temp_y{ (rand() % (rooms[rand_room].getRoom('h') - 40)) + rooms[rand_room].getRoom('y') };

		while (true) {
			for (unsigned int j{ 0 }; j < interactibles.size(); j++) {
				float tmp_x{ interactibles[j].getPosition().x }, tmp_y{ interactibles[j].getPosition().y };

				while ((temp_x == tmp_x && temp_y == tmp_y) || temp_x == -1 || temp_y == -1)
					temp_x = (rand() % (rooms[rand_room].getRoom('w') - 40)) + rooms[rand_room].getRoom('x'),
					temp_y = (rand() % (rooms[rand_room].getRoom('h') - 40)) + rooms[rand_room].getRoom('y');
				counter += 1;
			}

			if (interactibles.size() == counter) {
				int hidden{ rand() % 5 };

				interactibles.push_back(Interactible(temp_x, temp_y, !hidden));
				break;
			}
		}
	}
}

bool Floor::intersectStair(const sf::FloatRect& rect) {
	return stair.getGlobalBounds().intersects(rect);
}

bool Floor::intersectShop(const sf::FloatRect& rect) {
	return shop.getGlobalBounds().intersects(rect);
}
