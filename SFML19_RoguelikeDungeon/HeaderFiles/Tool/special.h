/**
*
* File: special.h
* Description: This class abstracts a special item/perk that players can get like increasing inventory limits.
*
*/

#ifndef SPECIAL_H
#define SPECIAL_H

#include "tool.h"
#include <functional>
#include <map>
#include <string>

class Special : public Tool {
private:
	std::function<void()> useSpecial = [](){};

public:
	static std::map<unsigned int, Special> specials;

	/**
	* Constructor for Special.
	* 
	* Parameter:
	* 	id: an unsigned int for the spell's id.
	* 	buy: an unsigned int for the spell's buy gold.
	* 	sell: an unsigned int for the spell's sell gold.
	*	abbre: a const char[3] for the items' name abbreviation.
	*	desc: a const char* for the item's description.
	*	name: a const char* for item's name.
	*/
	Special(unsigned int id, unsigned int buy, unsigned int sell,
		std::string abbre, std::string desc, std::string name, std::function<void()> useSpecial);

	static bool setup();

	void use();
};

#endif