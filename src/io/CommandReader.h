#ifndef COMMANDREADER_H
#define COMMANDREADER_H

#include <boost/fiber/fiber.hpp>
#include <boost/fiber/unbuffered_channel.hpp>

#include <optional>
#include <string>

class CommandLoop {
	public:
		CommandLoop();
		~CommandLoop();
		CommandLoop(const CommandLoop&) = delete;
		CommandLoop& operator=(const CommandLoop&) = delete;
		std::optional<std::string> waitForCommand();
	
	private:
		boost::fibers::unbuffered_channel<bool> command_channel_tx;
		boost::fibers::unbuffered_channel<std::string> command_channel_rx;
		boost::fibers::fiber read_command_loop_fiber;
};

#endif