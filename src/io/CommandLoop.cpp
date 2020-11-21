#include "CommandLoop.h"

#include "ConsoleOutputter.h"

#include <boost/fiber/buffered_channel.hpp>
#include <boost/fiber/fiber.hpp>

#include <chrono>
#include <iostream>
#include <optional>
#include <poll.h>
#include <readline.h>
#include <thread>

using namespace std::chrono_literals;
namespace fibers = boost::fibers;

void watch_stdin(fibers::buffered_channel<bool>& ping, fibers::buffered_channel<bool>& ack);
void handle_line(char* line);

CommandLoop::CommandLoop()
	: read_command_loop_fiber{&CommandLoop::read_command_loop, this}
{

}

CommandLoop::~CommandLoop() {
	read_command_loop_fiber.join();
}

std::optional<std::string> CommandLoop::waitForCommand() {
	// Tell the command fiber we want a command
	ready_for_command_channel.push(true);

	// Now wait for the command:
	std::string command;
	if(fibers::channel_op_status::success != command_channel.pop(command)) {
		return {};
	}
	return command;
}

std::optional<std::string> CommandLoop::command_received;

void CommandLoop::read_command_loop() {
	// Kick off a thread which uses poll to wait until there's stdin to read
	fibers::buffered_channel<bool> watch_stdin_thread_tx{2};
	fibers::buffered_channel<bool> watch_stdin_thread_rx{2};
	std::thread watch_stdin_thread{watch_stdin, std::ref(watch_stdin_thread_tx), std::ref(watch_stdin_thread_rx)};
	
	while(true) {
		// Wait for the main fiber to tell me that it wants a command
		bool garbage;
		if(ready_for_command_channel.pop(garbage) != fibers::channel_op_status::success) { break; }


		// Tell readline to put out a prompt:
		rl_callback_handler_install("> ", handle_line);
		prompted_readline = true;

		// Read a command:
		while(true) {
			// Tell the watch_stdin_thread that we're waiting on input ...
			watch_stdin_thread_tx.push(true);
			// ... then wait for the watch_stdin_thread to tell me there's something to read
			if(watch_stdin_thread_rx.pop(garbage) != fibers::channel_op_status::success) {
				// TODO does this need to break multiple levels?
				break;
			}
			
			// Have readline read a character ...
			rl_callback_read_char();

			// ... which may call the handler which sets the optional
			if(command_received) {
				break;
			}
		}

		prompted_readline = false;
		rl_clear_visible_line();
		rl_callback_handler_remove();

		std::string command = command_received.value();
		command_received.reset();
		command_channel.push(command);

		if(command == "exit" || command == "quit") { break; }
	}
	watch_stdin_thread_tx.close();
	watch_stdin_thread.join();
}

void watch_stdin(fibers::buffered_channel<bool>& rx, fibers::buffered_channel<bool>& tx) {
	struct pollfd fd;
	fd.fd = STDIN_FILENO;
	fd.events = POLLIN;
	while(true) {
		// Wait for the read_command_loop to tell us it's waiting on us:
		bool garbage;
		if(rx.pop(garbage) != fibers::channel_op_status::success) { break; }

		// Then wait until there's something in the stdin buffer
		poll(&fd, 1, -1);
		if(fd.revents & (POLLHUP | POLLERR | POLLNVAL)) { break; }
		if(!(fd.revents & POLLIN)) { continue; }

		// There's something to read, tell the read_command_loop
		tx.push(true);
	}
}

void handle_line(char* cline) {
	CommandLoop::command_received = cline;
	free(cline);
}