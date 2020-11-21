#include "IRoom.h"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

void Game::IRoom::describe(ConsoleOutputter& writer) const {
	innerDescribe(writer);
	printExits(writer);
}

void Game::IRoom::printExits(ConsoleOutputter& writer) const {
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
	writer.write(sstr.str());
}