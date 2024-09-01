/**
*
* File: stair.cpp
* Description: Contain implementations for the Stair class.
*
*/

#include "Floor/stair.h"
#include "Manager/texture_manager.h"
#include <Floor/floor_object.h>

Stair::Stair() : Floor_Object(Texture_Manager::stair) {}

Stair::Stair(float x, float y) : Floor_Object(x, y, Texture_Manager::stair) {}