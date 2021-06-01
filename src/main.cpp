#include "interfaces/IMap.h"
#include "interfaces/IRoomMaintainer.h"
#include "io/CommandLoop.h"
#include "io/ConsoleOutputter.h"
#include "io/map_display.h"
#include "io/Style.h"
#include "Map.h"
#include "MonasteryMazeRoomMaintainer.h"
#include "TimerManager.h"
#include "WildernessRoomMaintainer.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <memory>
#include <optional>
#include <readline.h>
#include <string>
#include <vector>

using namespace std::chrono_literals;

void startTimer(TimerManager& timerManager, ConsoleOutputter& writer) {
	TimerRequest timer = {
		.callback = [&writer]() { writer.write("It's been 15 seconds"); },
		.min = 15s,
		.max = 15s,
	};

	timerManager.addTimer(timer);

	auto called = std::make_shared<int>(0);
	RepeatingTimerRequest repeatRequest = {
		.callback = [called, &writer]() { writer.write("Timer"); return (++(*called) < 5); },
		.min = 4s,
		.max = 10000ms,
		.ignoreMinFirstTime = true,
	};

	timerManager.addTimer(repeatRequest);
}

int main(int argc, char** argv) {
	(void)argc, (void)argv;

	rl_initialize();

	CommandLoop commandLoop;
	ConsoleOutputter writer{std::cout, commandLoop};
	TimerManager timerManager;

	auto maintainer = Game::WildernessRoomMaintainer{};
	auto room = maintainer.getRoom(std::make_pair(10, 10));

	std::string exit;
	writer.write(room->describe());

	int commands = 0;

	std::optional<std::string> command_opt;
	while((command_opt = commandLoop.waitForCommand())) {
		std::string command = *command_opt;
		if(command == "exit" || command == "quit") { break; }
		auto exits = room->exitDescriptors();
		auto it = std::find_if(exits.begin(), exits.end(), [&command](auto exitDescriptor) { return command == exitDescriptor.name; });
		if(it == exits.end()) { continue; }

		room = maintainer.getRoom(it->id);
		// TODO bad
		auto wildernessRoom = std::dynamic_pointer_cast<Game::WildernessRoom>(room);
		Map map{wildernessRoom->location.first, wildernessRoom->location.second, maintainer};
		writer.write(displayMap(map));

		writer.write(room->describe());

		if(++commands == 5) {
			startTimer(timerManager, writer);
		}
	}
	
	return 0;
}