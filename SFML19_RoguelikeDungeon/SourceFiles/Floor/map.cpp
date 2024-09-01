/**
*
* File: map.cpp
* Description: Contain implementations for the Map class.
*
*/

#include "Floor/map.h"
#include <Floor/room.h>
#include <vector>

Map::Map() {}

Map::Map(std::vector<Room> &rooms) {
	Room first = rooms[0];
	int offsetX = first.getRoom('x') / 8 - 550;
	int offsetY = first.getRoom('y') / 8 - 350;

	for (Room rm : rooms) {
		rm.setPosSize(rm.getRoom('x') / 8 - offsetX, rm.getRoom('y') / 8 - offsetY, rm.getRoom('w') / 8, rm.getRoom('h') / 8);

		if (rm.existDoor())
			rm.setDoor(rm.getDoor('x') / 8 - offsetX, rm.getDoor('y') / 8 - offsetY, -1),
			rm.setDoor((rm.getDoor('r') % 2 == 0) ? 15 : 0, (rm.getDoor('r') % 2 == 0) ? 0 : 15, -2);

		map.push_back(rm);
	}
}

void Map::draw() {
	for (Room rm : map)
		rm.draw('r');

	for (Room rm : map)
		rm.draw('d');
}