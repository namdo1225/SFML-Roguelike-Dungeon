/**
*
* File: sf_manager.cpp
* Description: Contains the implementation of the SF_Manager class.
*
*/

#include "Manager/sf_manager.h"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

const float SF_Manager::DEFAULT_SCREEN_X = 1200.f;
const float SF_Manager::DEFAULT_SCREEN_Y = 800.f;

float SF_Manager::x;
float SF_Manager::y;

float SF_Manager::worldX;
float SF_Manager::worldY;

float SF_Manager::slotX;
float SF_Manager::slotY;

sf::RenderWindow SF_Manager::window = sf::RenderWindow();
sf::Event SF_Manager::event;

sf::Vector2i SF_Manager::mousePos;
sf::Vector2f SF_Manager::mouseWorld;

sf::View SF_Manager::viewUI{ sf::FloatRect(0, 0, DEFAULT_SCREEN_X, DEFAULT_SCREEN_Y) };
sf::View SF_Manager::viewWorld{ sf::FloatRect(0, 0, DEFAULT_SCREEN_X, DEFAULT_SCREEN_Y) };
sf::View SF_Manager::viewLog{ sf::FloatRect(0, 0, DEFAULT_SCREEN_X, DEFAULT_SCREEN_Y) };
sf::View SF_Manager::viewSlots{ sf::FloatRect(0, 0, DEFAULT_SCREEN_X, DEFAULT_SCREEN_Y) };
sf::View SF_Manager::viewMap{ sf::FloatRect(0, 0, DEFAULT_SCREEN_X, DEFAULT_SCREEN_Y) };

unsigned int SF_Manager::worldZoomLevel = 0;