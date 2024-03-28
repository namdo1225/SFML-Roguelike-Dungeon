/**
*
* File: stair.cpp
* Description: Contain implementations for the Stair class.
*
*/

#include "Floor/stair.h"
#include "Manager/texture_manager.h"

Stair::Stair() : Floor_Object(Texture_Manager::stair) {}

Stair::Stair(int x, int y) : Floor_Object(x, y, Texture_Manager::stair) {}