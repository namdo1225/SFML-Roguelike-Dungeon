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
#include <map>
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
				strtol(argv[13], NULL, 10)
			}
		);


		return 0;
	});
	return true;
}

Enemy::Enemy(int floor, unsigned int id, float x, float y, int hp) : 
	Floor_Object(x, y,
		Texture_Manager::tex_enemies.contains(id) ? Texture_Manager::tex_enemies[id] : Texture_Manager::tex_enemies[0]) {
	EnemyFull& info = enemies[id];

	stat = info.stat;

	stat.hp = hp != -1 ? hp : info.stat.hp + (info.growth.hpGrowth * floor);
	stat.atk = info.stat.atk + (info.growth.atkGrowth * floor);
	stat.def = info.stat.def + (info.growth.defGrowth * floor);
	stat.res = info.stat.res + (info.growth.resGrowth * floor);
	stat.exp = info.stat.exp + (info.growth.expGrowth * floor);

	constant = &info.growth;
}

int Enemy::damageEnemy(bool type, int amount) {
	int protect{ type ? stat.def : stat.res };
	int quantity = std::max(1, amount - protect);
	stat.hp -= quantity;
	return quantity;
}