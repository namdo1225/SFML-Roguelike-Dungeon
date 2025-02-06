/**
*
* File: enemy.cpp
* Description: Contains implementations of Enemy class.
*
*/

#include "Floor/enemy.h"
#include "Manager/texture_manager.h"
#include <cstdlib>
#include <Floor/floor_object.h>
#include <Manager/database_manager.h>
#include <Manager/game_manager.h>
#include <Manager/sf_manager.h>
#include <map>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <stat.h>
#include <string>
#include <utility>

std::map<unsigned int, EnemyFull> Enemy::enemies;

bool Enemy::setup() {
	enemies.clear();
	Database_Manager::executeSelect("SELECT * FROM enemies;", [](void* data, int argc, char** argv, char** azColName) -> int {
		unsigned int i = strtol(argv[0], NULL, 10);

		enemies[i] = EnemyFull(
			{
				strtol(argv[2], NULL, 10),
				strtol(argv[4], NULL, 10),
				strtol(argv[6], NULL, 10),
				strtol(argv[8], NULL, 10),
				strtol(argv[10], NULL, 10),
				strtol(argv[11], NULL, 10),
			},
			{
				argv[1],
				strtol(argv[0], NULL, 10),
				std::stof(argv[3]),
				std::stof(argv[5]),
				std::stof(argv[7]),
				std::stof(argv[9]),
				std::stof(argv[12]),
				strtol(argv[14], NULL, 10),
				(Attack)strtol(argv[13], NULL, 10)
			}
		);


		return 0;
	});
	return true;
}

Enemy::Enemy(unsigned int id, float x, float y, int hp) : 
	Floor_Object(x, y,
		Texture_Manager::tex_enemies.contains(id) ? Texture_Manager::tex_enemies[id] : Texture_Manager::tex_enemies[0]) {
	EnemyFull& info = enemies[id];

	unsigned int floor = Game_Manager::player.getFloor();

	stat = info.stat;

	stat.hp = hp != -1 ? hp : info.stat.hp + (info.growth.hpGrowth * floor);
	stat.atk = info.stat.atk + (info.growth.atkGrowth * floor);
	stat.def = info.stat.def + (info.growth.defGrowth * floor);
	stat.res = info.stat.res + (info.growth.resGrowth * floor);
	stat.exp = info.stat.exp + (info.growth.expGrowth * floor);

	constant = &info.growth;

	float rangeArea = (stat.range * 2 + 1) * SF_Manager::TILE;
	rangeBox.setSize(sf::Vector2f(rangeArea, rangeArea));
	rangeBox.setOutlineThickness(0);
}

int Enemy::damageEnemy(Attack type, int amount) {
	int protect{ type ? stat.def : stat.res };
	int quantity = std::max(1, amount - protect);
	stat.hp -= quantity;
	return quantity;
}

void Enemy::draw(bool range) {
	Floor_Object::draw();
	if (range)
		SF_Manager::window.draw(rangeBox);

}

bool Enemy::intersectsRange(const sf::FloatRect& rect) {
	return rangeBox.getGlobalBounds().intersects(rect);
}

void Enemy::setPosition(float x, float y) {
	sf::RectangleShape::setPosition(x, y);

	float rangeArea = (stat.range * 2 + 1) * SF_Manager::TILE;
	rangeBox.setPosition(x - stat.range * SF_Manager::TILE, y - stat.range * SF_Manager::TILE);
	rangeBox.setSize(sf::Vector2f(rangeArea, rangeArea));

}
