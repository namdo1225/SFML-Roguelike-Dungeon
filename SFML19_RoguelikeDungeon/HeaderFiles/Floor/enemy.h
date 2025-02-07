#include "floor_object.h"
#include <map>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <Shape/full_rectangle.h>
#include <stat.h>
#include <string>
#include <Manager/sf_manager.h>

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

	/** Minimum floor when enemy will appear.
	*/
	int minimumFloor = 1;
	Attack type = Physical;
};

/**
* A struct to contain an enemy's both scaled and constant stat.
*/
struct EnemyFull {
	EnemyStat stat;
	EnemyConstant growth;
};

/**
* An enemy which can attack/kill the player.
*/
class Enemy : public Floor_Object {
private:
	using sf::RectangleShape::setPosition;

	Full_Rectangle rangeBox = Full_Rectangle(-100.f, -100.f, SF_Manager::TILE, SF_Manager::TILE, false, true, sf::Color(255, 0, 0, 50), sf::Color::Transparent);

public:
	static std::map<unsigned int, EnemyFull> enemies;
	EnemyStat stat;
	EnemyConstant* constant;

	/**
	* Load enemies from the database.
	* 
	* Return:
	*	true if loading is successful.
	*/
	static bool setup();

	/**
	* Constructor for Enemy.
	*
	* Parameter:
	*	id: Enemy's id.
	*	x: Enemy's x position.
	*	y: Enemy's y position.
	* 	hp: Enemy's hp. If hp = -1, hp is calculated by floors.
	*/
	Enemy(unsigned int id, float x, float y, int hp = -1);

	/**
	* Getter for enemy's position.
	*
	* Parameter:
	*	type: Player's attack type.
	*	amount: Player's attack amount.
	*
	* Return:
	*	Enemy's new hp after being attacked by player.
	*/
	int damageEnemy(Attack type, int amount);

	/**
	* Draw enemy.
	* 
	* Parameter:
	*	range: Whether to draw enemy's range box.
	*/
	void draw(bool range);

	/**
	* Whether a rectangle intersects the enemy's range box.
	* 
	* Parameter:
	*	rect: The rectangle to consider.
	*/
	bool intersectsRange(const sf::FloatRect& rect);


	void setPosition(float x, float y);
};

#endif