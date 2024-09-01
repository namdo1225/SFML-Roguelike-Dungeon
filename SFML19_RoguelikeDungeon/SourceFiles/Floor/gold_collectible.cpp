/**
*
* File: gold_collectible.cpp
* Description: Contain implementations for the Gold_Collectible class.
*
*/

#include "Floor/gold_collectible.h"
#include "Manager/texture_manager.h"
#include <cstdlib>
#include <Floor/floor_object.h>

Gold_Collectible::Gold_Collectible() : Floor_Object(Texture_Manager::gold) {}

Gold_Collectible::Gold_Collectible(unsigned int floor, unsigned int amount, float x, float y) : Floor_Object(x, y, Texture_Manager::gold), 
amount((floor != 0) ? rand() % (5 * floor) + (floor * 0.25) + 1 : amount) {}

unsigned int Gold_Collectible::getGold() {
	return amount;
}