#include "IRoom.h"

#include <iostream>
#include <string>
#include <vector>

void Game::IRoom::describe() const {
	innerDescribe();
	std::cout << "\n";
	printExits();
	std::cout << "\n";
}

void Game::IRoom::printExits() const {
	auto exits = exitNames();
	if(exits.size() > 0) {
		std::cout << "Exits: ";
		std::cout << exits[0];
		for(size_t i = 1; i < exits.size(); ++i) {
			std::cout << ", " << exits[i];
		}
	} else {
		std::cout << "There are no exits.";
	}
}