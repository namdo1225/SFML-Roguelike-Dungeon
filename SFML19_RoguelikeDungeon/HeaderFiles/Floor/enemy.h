/**
* 
* File: enemy.h
* Description: This class abstracts an enemy which can attack/kill the player.
* 
*/

#ifndef ENEMY_H
#define ENEMY_H

#include <array>
#include "Floor/floor_object.h"

class Enemy : public Floor_Object {
private:
	unsigned int id = 0;

	// attack basically represent both str/mag.
	// 0: hp, 1: atk, 2: def, 3: res, 4: range, 5: exp
	int stats[6] = { 1, 1, 1, 1, 1, 1 };

	// true = physical attack, false = magical.
	bool type{ true };

	std::string name = "";

	/**
	* A helper function to set the stat of the enemy.
	*
	* Parameter:
	*	hp: an int for the enemy's hp.
	* 	atk: an int for the enemy's atk.
	* 	def: an int for the enemy's def.
	* 	res: an int for the enemy's res.
	* 	range: an int for the enemy's range.
	* 	exp: an int for the enemy's exp.
	*/
	void set_stat_h(int hp, int atk, int def, int res, int range, int exp);

public:
	/**
	* Constructor for Enemy.
	*
	* Parameter:
	*	hp: an int for the enemy's hp.
	*	floor: an int for the player's current floor.
	*	id: an int for enemy's id.
	*	x: float for enemy's x position.
	*	y: float for enemy's y position.
	*/
	Enemy(int hp, int floor, unsigned int id, float x, float y);

	/**
	* Getter for enemy's position.
	*
	* Parameter:
	*	atk_type: int. Player's attack type.
	*	amount: int. Player's attack amount.
	*
	* Return:
	*	an int of enemy's new hp after being attacked by player.
	*	2 for def/physical. 3 for res/magic.
	*/
	int set_hp(int atk_type, int amount);

	/**
	* Setter for enemy's stat
	*
	* Parameter:
	*	i: an int for enemy's stat type.
	*	j: an int for what to set the value of that stat type.
	*/
	void set_stat(unsigned int i, int j);

	/**
	* Getter for enemy's id.
	*
	* Return:
	*	an int of enemy's id.
	*/
	unsigned int get_id();

	/**
	* Getter for enemy's position.
	*
	* Parameter:
	*	i: an int for enemy's stat type.
	*
	* Return:
	*	an int for the value of the stat type.
	*/
	int get_stat(unsigned int i);

	/**
	* Getter for enemy's type.
	*
	* Return:
	*	an int of enemy's type.
	*/
	bool get_type();

	/**
	* Getter for enemy's name.
	*
	* Return:
	*	a string for enemy's name.
	*/
	std::string get_name();
};

#endif