#include "interfaces/IRoomMaintainer.h"
#include "io/CommandLoop.h"
#include "io/ConsoleOutputter.h"
#include "MonasteryMazeRoomMaintainer.h"
#include "TimerManager.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <poll.h>
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

	auto maintainer = Game::MonasteryMazeRoomMaintainer{};
	auto room = maintainer.getRoom("");

	std::string exit;
	room->describe(writer);

	int commands = 0;

	std::optional<std::string> command_opt;
	while((command_opt = commandLoop.waitForCommand())) {
		std::string command = *command_opt;
		if(command == "exit" || command == "quit") { break; }
		auto exits = room->exitDescriptors();
		auto it = std::find_if(exits.begin(), exits.end(), [&command](auto exitDescriptor) { return command == exitDescriptor.name; });
		if(it == exits.end()) { continue; }

		room = maintainer.getRoom(it->id);

		room->describe(writer);

		if(++commands == 5) {
			startTimer(timerManager, writer);
		}
	}
	
	return 0;
}