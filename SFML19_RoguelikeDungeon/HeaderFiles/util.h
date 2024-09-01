#include <vector>
#include <string>

#ifndef UTIL_H
#define UTIL_H

/**
* A class providing methods and members with useful functionalities for the whole program.
*/
class Util {
public:

	/**
	* Splits string by a delimiter.
	* Link: https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
	* 
	* Parameter:
	*	s: the string to split.
	*	delimiter: the delimiter.
	* 
	* Return:
	*	a list of separated strings.
	*/
	static std::vector<std::string> split(std::string s, std::string delimiter);
};

#endif