#include "io/CommandReader.h"
#include "interfaces/IRoomMaintainer.h"
#include "MonasteryMazeRoomMaintainer.h"

#include <algorithm>
#include <iostream>
#include <poll.h>
#include <vector>

int main(int argc, char** argv) {
	(void)argc, (void)argv;

	CommandLoop commandLoop;

	auto maintainer = Game::MonasteryMazeRoomMaintainer{};
	auto room = maintainer.getRoom("");

	std::string exit;
	room->describe();

	std::optional<std::string> command_opt;
	while((command_opt = commandLoop.waitForCommand())) {
		std::string command = *command_opt;
		if(command == "exit" || command == "quit") { break; }
		auto exits = room->exitDescriptors();
		auto it = std::find_if(exits.begin(), exits.end(), [&command](auto exitDescriptor) { return command == exitDescriptor.name; });

		room = maintainer.getRoom(it->id);

		room->describe();
	}
	
	return 0;
}