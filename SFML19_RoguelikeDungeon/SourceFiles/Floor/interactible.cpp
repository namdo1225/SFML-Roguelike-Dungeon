/**
*
* File: interactible.cpp
* Description: Contains the implementation for the Interactible class.
*
*/

#include "Floor/interactible.h"
#include "Manager/texture_manager.h"

Interactible::Interactible() : Floor_Object(Texture_Manager::interactible) {}

Interactible::Interactible(float x, float y, bool hidden) : Floor_Object(x, y, Texture_Manager::interactible) {
	this->hidden = hidden;
}

void Interactible::setInactive() {
	hidden = false;
	inactive = true;
	setTexture(&Texture_Manager::interactibleInactive);
}