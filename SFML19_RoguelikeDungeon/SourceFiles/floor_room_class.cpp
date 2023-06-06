/**
*
* File: floor_room_class.cpp
* Description: Contain implementations for Floor and Room classes.
*
*/

#include "floor_room_class.h"

sf::Texture Room::room_tex;
sf::Texture Stair::stair_tex;
sf::Texture Shop::shop_tex;
sf::Texture Gold_Collectible::gold_tex;
sf::Texture Collectible::col_tex;

Collectible::Collectible() {
	rect.setSize(sf::Vector2f(40, 40));
	rect.setFillColor(sf::Color::Yellow);
	rect.setTextureRect(sf::IntRect(0, 0, 40, 40));
	rect.setTexture(&col_tex);
}

Collectible::Collectible(int x, int y, unsigned int t_id) : Collectible() {
	id = t_id;
	rect.setPosition(x, y);
}

int Collectible::get_pos(char z) { return z == 'x' ? rect.getPosition().x : rect.getPosition().y; }

unsigned int Collectible::get_id() { return id; }

void Collectible::set_pos(int x, int y) { rect.setPosition(x, y); }

void Collectible::draw(sf::RenderWindow& window) { window.draw(rect); }

bool Collectible::load_texture() {
	if (!col_tex.loadFromFile("Texture\\GG_04_Items.png"))
		return false;
	return true;
}

Gold_Collectible::Gold_Collectible() {
	rect.setSize(sf::Vector2f(40, 40));
	rect.setFillColor(sf::Color::Yellow);
	rect.setTextureRect(sf::IntRect(0, 0, 40, 40));
	rect.setTexture(&gold_tex);
}

Gold_Collectible::Gold_Collectible(unsigned int floor, unsigned int t_amount, int x, int y) : Gold_Collectible() {
	amount = (floor != 0) ? rand() % (5 * floor) + (floor * 0.25) + 1 : t_amount;
	rect.setPosition(x, y);
}

unsigned int Gold_Collectible::get_amount() { return amount; }

void Gold_Collectible::set_pos(int x, int y) { rect.setPosition(x, y); }

int Gold_Collectible::get_pos(char z) { return (z == 'x') ? rect.getPosition().x : rect.getPosition().y; }

void Gold_Collectible::draw(sf::RenderWindow& window) { window.draw(rect); }

bool Gold_Collectible::load_texture() {
	if (!gold_tex.loadFromFile("Texture\\GG_01_Gold.png"))
		return false;
	return true;
}

Stair::Stair() {
	rect.setSize(sf::Vector2f(40, 40));
	rect.setFillColor(sf::Color::Yellow);
	rect.setTextureRect(sf::IntRect(0, 0, 40, 40));
	rect.setTexture(&stair_tex);
}

Stair::Stair(int x, int y) : Stair() {
	rect.setPosition(x, y);
}

bool Stair::load_texture() {
	if (!stair_tex.loadFromFile("Texture\\GG_05_Stair.png"))
		return false;
	return true;
}

Shop::Shop() {
	rect.setSize(sf::Vector2f(40, 40));
	rect.setFillColor(sf::Color::White);
	rect.setTextureRect(sf::IntRect(0, 0, 40, 40));
	rect.setTexture(&shop_tex);
}

Shop::Shop(int x, int y) : Shop() {
	rect.setPosition(x, y);
}

bool Shop::load_texture() {
	if (!shop_tex.loadFromFile("Texture\\GG_06_Shop.jpg"))
		return false;
	return true;
}

Room::Room() {
	rm_draw.setFillColor(sf::Color::White);
	rm_draw.setOutlineThickness(3.f);

	sf::Color rgb(rand() % 50 + 100, rand() % 75, rand() % 75);

	rm_draw.setOutlineColor(rgb);
	rm_draw.setTexture(&room_tex, false);
}

void Room::set_pos_and_size(int x, int y, int sx, int sy) {
	if (sx != -1 and sy != -1) {
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
	} else if (purpose == -2)
		dr_draw.setSize(sf::Vector2f(x, y));

	if (purpose >= -1)
		dr_draw.setPosition(x, y);
}

int Room::get_rm(char z) {
	if (z == 'x')
		return rm_draw.getPosition().x;
	else if (z == 'y')
		return rm_draw.getPosition().y;
	else if (z == 'w')
		return rm_draw.getSize().x;
	else if (z == 'h')
		return rm_draw.getSize().y;
	else if (z == '1')
		return rm_draw.getPosition().x + rm_draw.getSize().x;
	else if (z == '2')
		return rm_draw.getPosition().y + rm_draw.getSize().y;
	else if (z == '3')
		return rm_draw.getPosition().y + rm_draw.getSize().x + rm_draw.getSize().y;
}

int Room::get_door(char z) {
	if (z == 'x')
		return dr_draw.getPosition().x;
	else if (z == 'y')
		return dr_draw.getPosition().y;
	else if (z == 'w')
		return dr_draw.getSize().x;
	else if (z == 'h')
		return dr_draw.getSize().y;
	else if (z == 'r')
		return door_rotation;
	else if (z == '0')
		return doors[0];
	else if (z == '1')
		return doors[1];
	else if (z == '2')
		return doors[2];
	else if (z == '3')
		return doors[3];
}

bool Room::in_room(int x, int y, int x2, int y2) {
	if (x >= rm_draw.getPosition().x and y >= rm_draw.getPosition().y and
		x2 <= rm_draw.getPosition().x + rm_draw.getSize().x and y2 <= rm_draw.getPosition().y + rm_draw.getSize().y)
		return true;
	return false;
}

bool Room::door_exist() { return door; }

bool Room::touch_door(int x, int y, int x2, int y2) {
	// if door does not exist, return false.
	if (!door)
		return false;

	if (door_rotation % 2 == 0) {
		if (y == dr_draw.getPosition().y or y2 == dr_draw.getPosition().y)
			if (x >= dr_draw.getPosition().x and x2 <= dr_draw.getPosition().x + 120)
				return true;
		return false;
	}
	else {
		if (x == dr_draw.getPosition().x or x2 == dr_draw.getPosition().x)
			if (y >= dr_draw.getPosition().y and y2 <= dr_draw.getPosition().y + 120)
				return true;
		return false;
	}
}

bool Room::get_doors(unsigned int i) { return doors[i]; }

void Room::set_doors(unsigned int i, bool j) { doors[i] = j; }

void Room::draw(sf::RenderWindow& window, char d) { (d == 'r') ? window.draw(rm_draw) : window.draw(dr_draw); }

bool Room::load_texture() {
	if (!room_tex.loadFromFile("Texture\\GG_00_StonePath.jpg"))
		return false;
	room_tex.setRepeated(true);
	return true;
}

Floor::Floor() {
	// maximum floor size: 2160x2160, one square: 40x40

	int max_rm{ rand() % 10 + 2 };
	for (unsigned int i{ 0 }; i < max_rm; i++) {
		int sx{ (rand() % 6 + 4) * 120 }, sy{ (rand() % 6 + 4) * 120 },
			x{ (rand() % 26 + 36) * 120 }, y{ (rand() % 26 + 36) * 120 };

		while (!(x + sx <= 8640 and y + sy <= 8640 and x >= 4320 and y >= 4320))
			x = (rand() % 26 + 36) * 120, y = (rand() % 26 + 36) * 120,
			sx = (rand() % 6 + 4) * 120, sy = (rand() % 6 + 4) * 120;

		rooms.push_back(Room());

		if (i == 0) {
			sx += 480, sy += 480;
			rooms[i].set_pos_and_size(x, y, sx, sy);
			continue;
		}

		int rand_dr{ rand() % 4 }, rand_rm{ rand() % (static_cast<int>(rooms.size()) - 1) };

		// make sure room has doors available. Otherwise, switch room.
		while (rooms[rand_rm].get_doors(0) == 1 and rooms[rand_rm].get_doors(1) == 1 and
			rooms[rand_rm].get_doors(2) == 1 and rooms[rand_rm].get_doors(3) == 1)
			rand_rm = rand() % (rooms.size() - 1);

		// make sure new room's door has the old room's door available. Otherwise, switch door.
		std::array<unsigned int, 4> door = { 2, 3, 0, 1 };
		bool choose_door = true;
		while (choose_door) {
			rand_dr = rand() % 4;
			if (rooms[rand_rm].get_doors(door[rand_dr]) == 0) {
				rooms[rand_rm].set_doors(door[rand_dr], 1);
				choose_door = false;
			}
		}
		rooms[i].set_doors(rand_dr, 1);

		// set new room's position and size.
		int break_loop_index{ static_cast<int>(rooms.size()) - 1 }, break_loop_counter{ 0 }, remove{ 0 }, k{ 0 };
		bool conflict_room{ false };

		while (true) {
			if (remove >= 20) break;

			break_loop_counter = 0;
			if (rand_dr % 2 == 0)
				set_rm_pos_size(rand_dr, x, sx, y, sy, rooms[rand_rm].get_rm('x'), rooms[rand_rm].get_rm('w'), rooms[rand_rm].get_rm('y'), rooms[rand_rm].get_rm('h'), conflict_room, k);
			else if (rand_dr % 2 == 1)
				set_rm_pos_size(rand_dr, y, sy, x, sx, rooms[rand_rm].get_rm('y'), rooms[rand_rm].get_rm('h'), rooms[rand_rm].get_rm('x'), rooms[rand_rm].get_rm('w'), conflict_room, k);

			for (unsigned int j{ 0 }; j < rooms.size() - 1; j++) {
				if (x >= rooms[j].get_rm('1') or x + sx <= rooms[j].get_rm('x') or
					y >= rooms[j].get_rm('2') or y + sy <= rooms[j].get_rm('y')) {
					break_loop_counter += 1;
				}
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

		rooms[i].set_pos_and_size(x, y, sx, sy);

		// set door's x or y
		int rand_dr_x{ (rand() % (sx / 40) + (x / 40)) * 40 }, rand_dr_y{ (rand() % (sy / 40) + (y / 40)) * 40 };

		switch (rand_dr) {
		case 0:
			set_door_pos_1(rand_dr_y, y, 0); break;
		case 2:
			set_door_pos_1(rand_dr_y, y, sy); break;
		case 1:
			set_door_pos_1(rand_dr_x, x, sx); break;
		case 3:
			set_door_pos_1(rand_dr_x, x, 0); break;
		}

		// set door's x or y
		if (rand_dr % 2 == 0)
			set_door_pos_2(rand_dr_x, x, sx, rooms[rand_rm].get_rm('x'), rooms[rand_rm].get_rm('w'));
		else
			set_door_pos_2(rand_dr_y, y, sy, rooms[rand_rm].get_rm('y'), rooms[rand_rm].get_rm('h'));

		rooms[i].set_door(rand_dr_x, rand_dr_y, rand_dr);
	}

	make_stair();
	make_shop();
	make_map();
}

void Floor::set_rm_pos_size(int rand_door, int& rand_coord, int& rand_size, int& rand_coord_2,
	int& rand_size_2, int rand_room_coord, int rand_room_size, int rand_room_coord_2, int rand_room_size_2, bool& conflict_room, int& k) {
	(k < 3) ? k += 1 : k = 0;

	if (rand_door == 0 or rand_door == 3)
		while (rand_coord_2 != rand_room_coord_2 + rand_room_size_2)
			rand_coord_2 = rand_room_coord_2 + rand_room_size_2, rand_size_2 = (rand() % 6 + 4 - k) * 120;
	else if (rand_door == 1 or rand_door == 2) {
		while (rand_coord_2 + rand_size_2 != rand_room_coord_2) {
			int temp_2 = rand_room_coord_2 / 120;
			int temp_3 = rand() % 12 + (temp_2 - 12);
			int temp_4 = temp_2 - temp_3;

			rand_coord_2 = temp_3 * 120, rand_size_2 = temp_4 * 120;
		}
	}

	while ((!(rand_coord + rand_size < rand_room_coord + rand_room_size and rand_room_coord < rand_coord and rand_coord + rand_size - 120 >= rand_room_coord) and
		!(rand_coord + rand_size > rand_room_coord + rand_room_size and rand_coord > rand_room_coord and rand_coord + 120 <= rand_room_coord + rand_room_size) and
		!(rand_coord >= rand_room_coord and rand_coord + rand_size <= rand_room_coord + rand_room_size) and
		!(rand_coord <= rand_room_coord and rand_coord + rand_size >= rand_room_coord + rand_room_size)) or conflict_room) {

		rand_coord = (rand() % (rand_room_size / 120) + (rand_room_coord / 120)) * 120, rand_size = (rand() % 6 + 4 - k) * 120;
		conflict_room = false;
		(k < 3) ? k += 1 : k = 0;
	}
}

void Floor::set_door_pos_1(int& rand_door_coord, int rand_coord, int rand_size) { rand_door_coord = rand_coord + rand_size; }

void Floor::set_door_pos_2(int& rand_door_coord, int rand_coord, int rand_size, int rand_room_coord, int rand_room_size) {
	if (rand_coord + rand_size < rand_room_coord + rand_room_size and rand_coord < rand_room_coord and rand_coord + rand_size > rand_room_coord and rand_room_coord + 120 <= rand_coord + rand_size) {
		int tem_1{ rand_coord + rand_size - rand_room_coord };

		rand_door_coord = (rand() % (tem_1 / 120) + (rand_room_coord / 120)) * 120;

		if (rand_door_coord == rand_coord + rand_size)
			rand_door_coord -= 120;
	}
	else if (rand_coord + rand_size > rand_room_coord + rand_room_size and rand_coord > rand_room_coord and rand_coord < rand_room_coord + rand_room_size and rand_coord + 120 <= rand_room_coord + rand_room_size) {
		int tem_1{ rand_room_coord + rand_room_size - rand_coord };

		rand_door_coord = (rand() % (tem_1 / 120) + (rand_coord / 120)) * 120;

		if (rand_door_coord == rand_room_coord + rand_room_size)
			rand_door_coord -= 120;
	}
	else if (rand_coord >= rand_room_coord and rand_coord + rand_size <= rand_room_coord + rand_room_size)
		rand_door_coord = (rand() % (rand_size / 120) + (rand_coord / 120)) * 120;
	else if (rand_coord <= rand_room_coord and rand_coord + rand_size >= rand_room_coord + rand_room_size)
		rand_door_coord = (rand() % (rand_room_size / 120) + (rand_room_coord / 120)) * 120;
}

void Floor::make_stair() {
	int rand_room{ rand() % static_cast<int>(rooms.size()) };

	int x{ ((rand() % (rooms[rand_room].get_rm('h') / 40)) + (rooms[rand_room].get_rm('x') / 40)) * 40 },
		y{ ((rand() % (rooms[rand_room].get_rm('w') / 40)) + (rooms[rand_room].get_rm('y') / 40)) * 40 };

	while (!rooms[rand_room].in_room(x, y, x + 40, y + 40))
		x = ((rand() % (rooms[rand_room].get_rm('h') / 40)) + (rooms[rand_room].get_rm('x') / 40)) * 40,
		y = ((rand() % (rooms[rand_room].get_rm('w') / 40)) + (rooms[rand_room].get_rm('y') / 40)) * 40;

	stair = Stair(x, y);
}

void Floor::make_shop() {
	if ((rand() % 10) != 0) return;

	int rand_room{ rand() % static_cast<int>(rooms.size()) };

	int x = ((rand() % (rooms[rand_room].get_rm('w') / 40)) + (rooms[rand_room].get_rm('x') / 40)) * 40,
	y = ((rand() % (rooms[rand_room].get_rm('h') / 40)) + (rooms[rand_room].get_rm('y') / 40)) * 40;

	while ((stair.rect.getPosition().x == x and stair.rect.getPosition().y == y) or !rooms[rand_room].in_room(x, y, x + 40, y + 40))
		x = ((rand() % (rooms[rand_room].get_rm('w') / 40)) + (rooms[rand_room].get_rm('x') / 40)) * 40,
		y = ((rand() % (rooms[rand_room].get_rm('h') / 40)) + (rooms[rand_room].get_rm('y') / 40)) * 40;

	shop = Shop(x, y);
	fl_shop = true;
}

int Floor::get_shop_pos(char z) { return z == 'x' ? shop.rect.getPosition().x : shop.rect.getPosition().y; }

int Floor::get_stair_pos(char z) { return z == 'x' ? stair.rect.getPosition().x : stair.rect.getPosition().y; }

bool Floor::shop_exist() { return fl_shop; }

void Floor::draw(sf::RenderWindow& window) {
	for (Room rm : rooms)
		rm.draw(window, 'r');

	for (Room rm : rooms)
		rm.draw(window, 'd');

	window.draw(stair.rect);

	if (fl_shop)
		window.draw(shop.rect);

	for (Gold_Collectible gold : golds)
		gold.draw(window);

	for (Collectible col : collectibles)
		col.draw(window);
}

void Floor::set_shop_pos(int x, int y) { shop.rect.setPosition(x, y); }

void Floor::set_stair_pos(int x, int y) { stair.rect.setPosition(x, y); }

void Floor::load_room(int x, int y, int sx, int sy) { rooms.push_back(Room()); rooms.back().set_pos_and_size(x, y, sx, sy); }

void Floor::load_door(int x, int y, int rot, int slot0, int slot1, int slot2, int slot3) { 
	rooms.back().set_door(x, y, rot); 
	rooms.back().set_doors(0, slot0);
	rooms.back().set_doors(1, slot1);
	rooms.back().set_doors(2, slot2);
	rooms.back().set_doors(3, slot3);
}

void Floor::load_stair(int x, int y) { stair = Stair(x, y); }

void Floor::load_collectible(int x, int y, unsigned int id) { collectibles.push_back(Collectible(x, y, id)); }

void Floor::load_gold(int x, int y, unsigned int amount) { golds.push_back(Gold_Collectible(0, amount, x, y)); }

void Floor::load_shop(int x, int y) { shop = Shop(x, y); }

void Floor::make_collectible(unsigned int floor) {
	int rand_items{ rand() % 6 + static_cast<int>(floor * 0.25) };

	for (unsigned int i{ 0 }; i < rand_items; i++) {
		int temp_x{ -1 }, temp_y{ -1 }, rand_room{ rand() % static_cast<int>(rooms.size()) }, counter{ 0 };

		bool loop{ true };
		while (loop) {
			for (Collectible col : collectibles) {
				int tmp_x{ col.get_pos('x') }, tmp_y{ col.get_pos('y') };

				while (temp_x == tmp_x and temp_y == tmp_y or temp_x == -1 or temp_y == -1)
					temp_x = ((rand() % (rooms[rand_room].get_rm('w') / 40)) + (rooms[rand_room].get_rm('x') / 40)) * 40,
					temp_y = ((rand() % (rooms[rand_room].get_rm('h') / 40)) + (rooms[rand_room].get_rm('y') / 40)) * 40;
				counter += 1;
			}

			if (collectibles.size() == counter) {
				unsigned int item{ static_cast<unsigned int>(rand() % 3 + 1) };

				if (item > 2)
					item = static_cast<unsigned int>(rand() % 6 + 1);
				loop = false;
				collectibles.push_back(Collectible(temp_x, temp_y, item));
			}
		}
	}
}

void Floor::make_gold(unsigned int floor) {
	int rand_gold{ rand() % 6 + static_cast<int>(floor * 0.50) };

	for (unsigned int i{ 0 }; i < rand_gold; i++) {
		int temp_x{ -1 }, temp_y{ -1 }, rand_room{ rand() % static_cast<int>(rooms.size()) }, counter{ 0 };

		bool break_loop{ true };
		while (break_loop) {
			for (unsigned int j{ 0 }; j < golds.size(); j++) {
				int tmp_x{ golds[j].get_pos('x') }, tmp_y{ golds[j].get_pos('y') };

				while ((temp_x == tmp_x and temp_y == tmp_y) or temp_x == -1 or temp_y == -1) 
					temp_x = ((rand() % (rooms[rand_room].get_rm('w') / 40)) + (rooms[rand_room].get_rm('x') / 40)) * 40,
					temp_y = ((rand() % (rooms[rand_room].get_rm('h') / 40)) + (rooms[rand_room].get_rm('y') / 40)) * 40;
				counter += 1;
			}

			if (golds.size() == counter) {
				break_loop = false;
				golds.push_back(Gold_Collectible(floor, 0, temp_x, temp_y));
			}
		}
	}
}

void Floor::make_map() {
	for (Room rm : rooms) {
		rm.set_pos_and_size(rm.get_rm('x') / 8 - 300, rm.get_rm('y') / 8 - 400, rm.get_rm('w') / 8, rm.get_rm('h') / 8);

		if (rm.door_exist())
			rm.set_door(rm.get_door('x') / 8 - 300, rm.get_door('y') / 8 - 400, -1),
			rm.set_door((rm.get_door('r') % 2 == 0) ? 15 : 0, (rm.get_door('r') % 2 == 0) ? 0 : 15, -2);

		rooms_map.push_back(rm);
	}
}

void Floor::draw_map(sf::RenderWindow& window) {
	for (Room rm : rooms_map)
		rm.draw(window, 'r');

	for (Room rm : rooms_map)
		rm.draw(window, 'd');
}