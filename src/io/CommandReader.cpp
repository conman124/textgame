#include "CommandReader.h"

#include <boost/fiber/buffered_channel.hpp>
#include <boost/fiber/fiber.hpp>

#include <iostream>
#include <poll.h>
#include <thread>

namespace fibers = boost::fibers;

void watch_stdin(fibers::buffered_channel<bool>& ping, fibers::buffered_channel<bool>& ack);
void read_command_loop(fibers::unbuffered_channel<bool>& rx, fibers::unbuffered_channel<std::string>& tx);

CommandLoop::CommandLoop()
	: command_channel_tx{}
	, command_channel_rx{}
	, read_command_loop_fiber{read_command_loop, std::ref(command_channel_tx), std::ref(command_channel_rx)}
{

}

CommandLoop::~CommandLoop() {
	read_command_loop_fiber.join();
}

std::optional<std::string> CommandLoop::waitForCommand() {
	// Tell the command fiber we want a command
	command_channel_tx.push(true);

	// Now wait for the command:
	std::string command;
	if(fibers::channel_op_status::success != command_channel_rx.pop(command)) {
		return {};
	}
	return command;
}

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

void read_command_loop(fibers::unbuffered_channel<bool>& rx, fibers::unbuffered_channel<std::string>& tx) {

	// Kick off a thread which uses poll to wait until there's stdin to read
	fibers::buffered_channel<bool> ping{2};
	fibers::buffered_channel<bool> ack{2};
	std::thread watch_stdin_thread{watch_stdin, std::ref(ping), std::ref(ack)};
	while(true) {
		// Wait for the main fiber to tell me that it wants a command
		bool garbage;
		if(rx.pop(garbage) != fibers::channel_op_status::success) { break; }

		// Wait for the watch_stdin_thread to tell me there's something to read
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