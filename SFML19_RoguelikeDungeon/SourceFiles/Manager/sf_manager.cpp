/**
*
* File: sf_manager.cpp
* Description: Contains the implementation of the SF_Manager class.
*
*/

#include "Manager/sf_manager.h"
#include "Manager/setting_manager.h"

const char* SF_Manager::FULL_STATS[] = { "Health", "Mana", "Strength", "Magic", "Defense", "Resistance" };

float SF_Manager::x;
float SF_Manager::y;

float SF_Manager::world_x;
float SF_Manager::world_y;

sf::RenderWindow SF_Manager::window = sf::RenderWindow();
sf::Event SF_Manager::event;

sf::Vector2i SF_Manager::mouse_pos;
sf::Vector2f SF_Manager::mouse_world;

sf::View SF_Manager::viewUI{ sf::FloatRect(0, 0, 1200, 800) };
sf::View SF_Manager::viewWorld{ sf::FloatRect(0, 0, 1200, 800) };
sf::View SF_Manager::viewLog{ sf::FloatRect(0, 0, 1200, 800) };