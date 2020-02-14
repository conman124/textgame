#include "interfaces/IRoomMaintainer.h"
#include "MonasteryMazeRoomMaintainer.h"

#include <algorithm>
#include <iostream>

int main(int argc, char** argv) {
	(void)argc, (void)argv;

	auto maintainer = Game::MonasteryMazeRoomMaintainer{};
	auto room = maintainer.getRoom("");

	std::string exit;
	room->describe();

	while(std::getline(std::cin, exit)) {
		auto exits = room->exitDescriptors();
		auto it = std::find_if(exits.begin(), exits.end(), [exit](auto exitDescriptor) { return exit == exitDescriptor.name; });

		room = maintainer.getRoom(it->id);

		room->describe();
	}

	
	return 0;
}