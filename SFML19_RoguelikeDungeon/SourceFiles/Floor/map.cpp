/**
*
* File: map.cpp
* Description: Contain implementations for the Map class.
*
*/

#include "Floor/map.h"

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
}