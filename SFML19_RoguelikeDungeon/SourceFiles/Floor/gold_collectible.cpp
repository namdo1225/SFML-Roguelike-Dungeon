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
#include <State/player_state.h>
#include <utility>

Gold_Collectible::Gold_Collectible() : Floor_Object(Texture_Manager::gold) {}

Gold_Collectible::Gold_Collectible(float x, float y, unsigned int amount) : Floor_Object(x, y, Texture_Manager::gold),
amount(!amount ? rand() % (5 * std::max(Player_State::player.getFloor(), 1U)) + (Player_State::player.getFloor() * 0.25) + 1 : amount) {}

unsigned int Gold_Collectible::getGold() {
	return amount;
}