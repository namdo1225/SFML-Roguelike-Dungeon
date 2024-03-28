/**
*
* File: interactible.cpp
* Description: Contains the implementation for the Interactible class.
*
*/

#include "Floor/interactible.h"
#include "Manager/texture_manager.h"

Interactible::Interactible() : Floor_Object(Texture_Manager::interactible) {}

Interactible::Interactible(float x, float y) : Floor_Object(x, y, Texture_Manager::interactible) {}