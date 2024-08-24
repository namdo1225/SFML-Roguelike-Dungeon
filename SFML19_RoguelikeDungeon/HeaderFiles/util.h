#include <vector>
#include <string>

#ifndef UTIL_H
#define UTIL_H

class Util {
public:
	// https ://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
	static std::vector<std::string> split(std::string s, std::string delimiter);
};

#endif