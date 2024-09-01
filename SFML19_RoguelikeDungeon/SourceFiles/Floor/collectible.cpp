/**
*
* File: collectible.cpp
* Description: Contain implementations for the Collectible class.
*
*/

#include "Floor/collectible.h"
#include "Manager/texture_manager.h"
#include <Floor/floor_object.h>

Collectible::Collectible() : Floor_Object(Texture_Manager::collectible) {}

Collectible::Collectible(float x, float y, unsigned int id) : Floor_Object(x, y, Texture_Manager::collectible), id(id) {}

unsigned int Collectible::getID() { return id; }