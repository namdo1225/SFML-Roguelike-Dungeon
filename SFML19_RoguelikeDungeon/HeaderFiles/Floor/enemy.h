#include "floor_object.h"
#include <map>
#include <string>

#ifndef ENEMY_H
#define ENEMY_H

/**
* Represents an enemy's stat as affected by the floor it's on.
*/
struct EnemyStat {
	int hp = 1;
	int atk = 1;
	int def = 1;
	int res = 1;
	int range = 1;
	int exp = 1;
};

/**
* Represents an enemy's constant stat such as name.
*/
struct EnemyConstant {
	std::string name;
	unsigned int id = 0;

	float hpGrowth;
	float atkGrowth;
	float defGrowth;
	float resGrowth;
	float expGrowth;

	int minimumFloor = 1;
	bool type = true;
};

/**
* A struct to contain an enemy's both scaled and constant stat.
*/
struct EnemyFull {
	EnemyStat stat;
	EnemyConstant growth;
};

/**
* This class abstracts an enemy which can attack/kill the player.
*/
class Enemy : public Floor_Object {
public:
	static std::map<unsigned int, EnemyFull> enemies;
	EnemyStat stat;
	EnemyConstant* constant;

	static bool setup();

	/**
	* Constructor for Enemy.
	*
	* Parameter:
	*	floor: an int for the player's current floor.
	*	id: an int for enemy's id.
	*	x: float for enemy's x position.
	*	y: float for enemy's y position.
	* 	hp: an int for the enemy's hp.
	*/
	Enemy(int floor, unsigned int id, float x, float y, int hp = -1);

	/**
	* Getter for enemy's position.
	*
	* Parameter:
	*	type: bool. Player's attack type.
	*	amount: int. Player's attack amount.
	*
	* Return:
	*	an int of enemy's new hp after being attacked by player.
	*/
	int damageEnemy(bool type, int amount);
};

#endif