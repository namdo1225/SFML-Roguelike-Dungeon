#include "tool.h"
#include <functional>
#include <map>
#include <string>

#ifndef SPECIAL_H
#define SPECIAL_H

/**
* This class abstracts a special item/perk that players can get like increasing inventory limits.
*/
class Special : public Tool {
private:
	std::function<void()> useSpecial = [](){};

public:
	static std::map<unsigned int, Special> specials;

	/**
	* Constructor for Special.
	* 
	* Parameter:
	* 	id: The special's id.
	* 	buy: The special's buy gold.
	* 	sell: The special's sell gold.
	*	abbre: The special's name abbreviation.
	*	desc: The special's description.
	*	name: The special's name.
	*	usespecial: Function that will be called if special is purchased.
	*/
	Special(unsigned int id, unsigned int buy, unsigned int sell,
		std::string abbre, std::string desc, std::string name, std::function<void()> useSpecial);

	/**
	* Set up standard specials.
	* 
	* Return:
	*	true if specials are loaded successfully.
	*/
	static bool setup();

	/**
	* Use the special.
	*/
	void use();
};

#endif