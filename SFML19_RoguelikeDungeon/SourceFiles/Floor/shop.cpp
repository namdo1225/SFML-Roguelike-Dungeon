/**
*
* File: shop.cpp
* Description: Contain implementations for the Shop class.
*
*/

#include "Floor/shop.h"
#include "Manager/texture_manager.h"
#include <Floor/floor_object.h>

Shop::Shop() : Floor_Object(Texture_Manager::shop) {}

Shop::Shop(float x, float y) : Floor_Object(x, y, Texture_Manager::shop) {}