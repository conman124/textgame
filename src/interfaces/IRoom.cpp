#include "IRoom.h"

#include <cstddef>
#include <string>
#include <sstream>
#include <vector>

std::string Game::IRoom::describe() const {
	std::string out = innerDescribe();
	out += "\n";
	out += stringifyExits();
	return out;
}

std::string Game::IRoom::stringifyExits() const {
	auto exits = exitNames();
	std::stringstream sstr;
	if(exits.size() > 0) {
		sstr << "Exits: ";
		sstr << exits[0];
		for(size_t i = 1; i < exits.size(); ++i) {
			sstr << ", " << exits[i];
		}
	} else {
		sstr << "There are no exits.";
	}
	return sstr.str();
}