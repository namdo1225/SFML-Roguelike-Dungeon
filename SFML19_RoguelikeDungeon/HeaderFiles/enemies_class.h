/**
* 
* File: enemies_class.h
* Description: This class abstracts an enemy which can attack/kill the player.
* 
*/

#ifndef ENEMY_H
#define ENEMY_H

#include "..\SFML-2.5.1/include/SFML/Graphics.hpp"
#include <array>

class Enemy : sf::RectangleShape {
public:

	static const unsigned int num_textures = 4;

	/**
	* Constructor for Enemy.
	*
	* Parameter:
	*	hp: an int for the enemy's hp.
	*	floor: an int for the player's current floor.
	*	t_id: an int for enemy's id.
	*	x: int for enemy's x position.
	*	y: int for enemy's y position.
	*/
	Enemy(int hp, int floor, unsigned int t_id, int x, int y);

	/**
	* Getter for enemy's position.
	*
	* Parameter:
	*	z: 'x' or 'y' char. Get enemy's x or y position.
	*
	* Return:
	*	an int of enemy's x or y position.
	*/
	int get_pos(char z);

	/**
	* Getter for enemy's position.
	*
	* Parameter:
	*	atk_type: int. Player's attack type.
	*	amount: int. Player's attack amount.
	*
	* Return:
	*	an int of enemy's new hp after being attacked by player. 2 for def/physical. 3 for res/magic.
	*/
	int set_hp(int atk_type, int amount);

	/**
	* Setter for enemy's position.
	*
	* Parameter:
	*	x: an int for enemy's x position.
	*	y: an int for enemy's y position.
	*/
	void set_pos(int x, int y);

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
	* Draw the enemy on the screen.
	*
	* Parameter:
	*	window: a sf::RenderWindow object to draw the enemy on.
	*/
	void draw(sf::RenderWindow& window);

	/**
	* Method to load texture.
	*
	* Return:
	*	a bool. true if texture successfully loads.
	*/
	static bool load_texture();
private:
	// 1: zombie, 2: skeleton, 3: mage, 4: bandit
	unsigned int id;

	static std::array<sf::Texture, num_textures> texs;

	// attack basically represent both str/mag.
	// 0: hp, 1: atk, 2: def, 3: res, 4: range, 5: exp
	std::array<int, 6> stats = {1, 1, 1, 1, 1, 1};

	// choose whether it is a physical or magical attack. True = physical, false = magical.
	bool type{ true };

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

};

#endif