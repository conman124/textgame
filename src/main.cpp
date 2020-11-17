#include "io/CommandReader.h"
#include "interfaces/IRoomMaintainer.h"
#include "MonasteryMazeRoomMaintainer.h"
#include "TimerManager.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <poll.h>
#include <vector>

using namespace std::chrono_literals;

void startTimer(TimerManager& timerManager) {
	TimerRequest timer = {
		.callback = []() { std::cout << "It's been 15 seconds\n"; },
		.min = 15s,
		.max = 15s,
	};

	timerManager.addTimer(timer);

	auto called = std::make_shared<int>(0);
	RepeatingTimerRequest repeatRequest = {
		.callback = [called]() { std::cout << "Timer\n"; return (++(*called) < 5); },
		.min = 4s,
		.max = 10000ms,
		.ignoreMinFirstTime = true,
	};

	timerManager.addTimer(repeatRequest);
}

int main(int argc, char** argv) {
	(void)argc, (void)argv;

	CommandLoop commandLoop;
	TimerManager timerManager;

	auto maintainer = Game::MonasteryMazeRoomMaintainer{};
	auto room = maintainer.getRoom("");

	std::string exit;
	room->describe();

	int commands = 0;

	std::optional<std::string> command_opt;
	while((command_opt = commandLoop.waitForCommand())) {
		std::string command = *command_opt;
		if(command == "exit" || command == "quit") { break; }
		auto exits = room->exitDescriptors();
		auto it = std::find_if(exits.begin(), exits.end(), [&command](auto exitDescriptor) { return command == exitDescriptor.name; });

		room = maintainer.getRoom(it->id);

		room->describe();

		if(++commands == 5) {
			startTimer(timerManager);
		}
	}
	
	return 0;
}