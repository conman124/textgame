#include "interfaces/IRoomMaintainer.h"
#include "MonasteryMazeRoomMaintainer.h"

#include <boost/fiber/all.hpp>

#include <algorithm>
#include <iostream>
#include <poll.h>
#include <vector>

namespace fibers = boost::fibers;

void watch_stdin(fibers::buffered_channel<bool>& ping, fibers::buffered_channel<bool>& ack) {
	struct pollfd fd;
	fd.fd = STDIN_FILENO;
	fd.events = POLLIN;
	while(true) {
		poll(&fd, 1, -1);
		if(fd.revents & (POLLHUP | POLLERR | POLLNVAL)) { break; }
		if(!(fd.revents & POLLIN)) { continue; }

		// There's something to read, tell the read_command_loop ...
		ping.push(true);
		// ... and wait for a response
		bool garbage;
		if(ack.pop(garbage) != fibers::channel_op_status::success) { break; }
	}
}

void read_command_loop(fibers::unbuffered_channel<std::string>& tx) {

	// Kick off a thread which uses poll to wait until there's stdin to read
	fibers::buffered_channel<bool> ping{2};
	fibers::buffered_channel<bool> ack{2};
	std::thread watch_stdin_thread{watch_stdin, std::ref(ping), std::ref(ack)};
	while(true) {
		// Wait for the watch_stdin_thread to tell me there's something to read
		bool garbage;
		if(ping.pop(garbage) != fibers::channel_op_status::success) { break; }

		std::string command;
		std::getline(std::cin, command);
		tx.push(command);

		if(command == "exit" || command == "quit") { break; }
		ack.push(true);
	}
	ack.close();
	watch_stdin_thread.join();
}

int main(int argc, char** argv) {
	(void)argc, (void)argv;

	fibers::unbuffered_channel<std::string> command_rx;
	fibers::fiber read_command_loop_thread(read_command_loop, std::ref(command_rx));

	auto maintainer = Game::MonasteryMazeRoomMaintainer{};
	auto room = maintainer.getRoom("");

	std::string exit;
	room->describe();

	std::string command;
	while(fibers::channel_op_status::success == command_rx.pop(command)) {
		if(command == "exit" || command == "quit") { break; }
		std::string exit = command;
		auto exits = room->exitDescriptors();
		auto it = std::find_if(exits.begin(), exits.end(), [exit](auto exitDescriptor) { return exit == exitDescriptor.name; });

		room = maintainer.getRoom(it->id);

		room->describe();
	}
	read_command_loop_thread.join();
	
	return 0;
}