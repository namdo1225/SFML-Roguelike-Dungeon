#ifndef PLAYER_H
#define PLAYER_H

#include "effect.h"
#include "stat.h"
#include <array>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <string>
#include <vector>

/**
* Class to contain player's information and helps process the player's actions.
*/
class Player : public sf::RectangleShape {
private:
	std::vector<Effect> effects;

	std::string name = "Player";

	/** value: false = free, true = stuck, can't move
	* index: 0 = up, 1 = left, 2 = down, 3 = right
	*/
	std::array<bool, 4> stuck{ false, false, false, false };

	unsigned int points{ 10 }, level{ 1 }, curEXP{ 0 }, lvlUpEXP{ 10 }, floor{ 1 }, gold{ 0 }, maxItem{ 10 };

	// MAX_HP, MAX_MP, STR, MAG, DEF, RES, HP, MP
	std::array<long, 8> stat = { 10, 5, 0, 0, 0, 0, 10, 5};

public:

	/**
	* Constructor for Player.
	*/
	Player();

	/**
	* Getter for player's level.
	*
	* Return:
	*	Player's level.
	*/
	unsigned int getLVL();

	/**
	* Getter for player's level.
	*
	* Return:
	*	Player's level.
	*/
	unsigned int getCurEXP();

	/**
	* Getter for player's exp needed for level up.
	*
	* Return:
	*	Player's level-up exp.
	*/
	unsigned int getLVLUpEXP();

	/**
	* Getter for player's gold amount.
	*
	* Return:
	*	Player's gold amount.
	*/
	unsigned int getGold();

	/**
	* Getter for player's stat points.
	*
	* Return:
	*	Player's stat points.
	*/
	unsigned int getStatPts();

	/**
	* Getter for player's max item slots.
	*
	* Return:
	*	Player's max item slots.
	*/
	unsigned int getMaxItems();

	/**
	* Getter for player's current floor.
	*
	* Return:
	*	Player's current floor.
	*/
	unsigned int getFloor();

	/**
	* Getter for player's stat.
	*
	* Parameter:
	*	stat: The stat type.
	* 
	* Return:
	*	The value of stat type.
	*/
	long getStat(Stat stat);

	/**
	* Getter for player's position.
	*
	* Parameter:
	*	z: 'x' for player x position. 'y' for y position.
	* 
	* Return:
	*	the player's position.
	*/
	int getPos(char z);

	/**
	* Getter for player's name
	*
	* Return:
	*	the player's name.
	*/
	std::string getName();

	/**
	* Check if player is stuck.
	*
	* Parameter:
	*	i: index for player's mobility in a certain direction.
	* 
	* Return:
	*	true if player is dead.
	*/
	bool isStuck(unsigned int i);

	/**
	* Check if player is dead (hp is 0 or smaller).
	*
	* Return:
	*	true if player is dead.
	*/
	bool isDead();

	/**
	* Method to set player's status effect.
	*
	* Parameter:
	*	stat: Stat type to affect.
	*	quantity:How much to affect that stat.
	*	longevity: How many turns the effect will apply for.
	*	current: How many turns the effect has left (useful for loading from saves).
	*/
	void setEffect(Stat stat, int quantity, unsigned int longevity, unsigned int current = 0);

	/**
	* Setter for player's name.
	*
	* Parameter:
	*	name: the player's new name.
	*/
	void setName(std::string name);

	/**
	* Setter for player's current floor.
	*
	* Parameter:
	*	num: the player's new floor.
	*/
	void setFloor(unsigned int num);

	/**
	* Setter for player's current level.
	*
	* Parameter:
	*	lvl: the player's new level.
	*/
	void setLVL(unsigned int lvl);

	/**
	* Setter for player's gold amount.
	*
	* Parameter:
	*	gd: the player's new gold amount.
	*/
	void setGold(unsigned int gd);

	/**
	* Setter for player's stat points.
	*
	* Parameter:
	*	pts: the player's new stat points.
	*/
	void setStatPoint(unsigned int pts);

	/**
	* Setter for player's current exp.
	*
	* Parameter:
	*	curr_exp: the player's new exp.
	*/
	void setCurEXP(unsigned int curr_exp);

	/**
	* Setter for player's exp needed to level up.
	*
	* Parameter:
	*	lvl_exp: the new level up exp.
	*/
	void setLVLUpEXP(unsigned int lvl_exp);

	/**
	* Setter for player's max item slots.
	*
	* Parameter:
	*	slots: player's new max item slots.
	*/
	void setMaxItem(unsigned int slots);

	/**
	* Setter for player's stat.
	*
	* Parameter:
	*	type: Stat type.
	*	num: the new value for the stat type.
	*/
	void setStat(Stat type, unsigned int num);

	/**
	* Setter for player's position.
	*
	* Parameter:
	*	x: player's x position.
	* 	y: player's y position.
	*/
	void setPos(int x, int y);

	/**
	* Setter for player's stuck direction (they cannot move that way if stuck 
	* in that direction).
	*
	* Parameter:
	*	i: indexing of direction.
	*		0 = up, 1 = left, 2 = down, 3 = right
	*	j: true if stuck.
	*/
	void setStuck(unsigned int i, bool j);

	/**
	* Use player's mp & calculate remaining mp.
	* 
	* Parameter:
	*	quantity: an int for the amount of mp that will be used.
	*/
	void useMP(int quantity);

	/**
	* Attack player & calculate remaining hp.
	*
	* Parameter:
	*	type: The attack type.
	*	quantity: The damage amount.
	* 
	* Return:
	*	How much the player is actually damaged by.
	*/
	int hurtPlayer(Attack type, int quantity);
	
	/**
	* Spend player's gold.
	*
	* Parameter:
	*	quantity: How much money to spend.
	*
	* Return:
	*	true if the player has that amount to spend/
	*/
	bool useGold(unsigned int quantity);

	/**
	* Method to reset object's members to default value.
	* 
	* Parameter:
	*	cheat: true if player is powerful upon start.
	*	replaceName: true to replace name with default option.
	*/
	void reset(bool cheat = false, bool replaceName = true);

	/**
	* Copy player's stat for general usage.
	*
	* Parameter:
	*	stats: The stats to copy.
	*/
	void copyStat(std::array<long, 8>& stats);

	/**
	* Handles effects on player.
	*/
	void useEffect();

	/**
	* Resets effects on player.
	*/
	void resetEffect();

	/**
	* Getter for effects.
	* 
	* Return:
	*	list of effects
	*/
	std::vector<Effect> getEffect();

	/**
	* Gets player's rectangle.
	*
	* Return:
	*	Player's rectangle.
	*/
	sf::FloatRect getRect();
};

#endif