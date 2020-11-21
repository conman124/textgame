#ifndef COMMANDLOOP_H
#define COMMANDLOOP_H

#include <boost/fiber/fiber.hpp>
#include <boost/fiber/unbuffered_channel.hpp>

#include <optional>
#include <string>

class CommandLoop {
	public:
		CommandLoop();
		~CommandLoop();
		CommandLoop(const CommandLoop&) = delete;
		CommandLoop(CommandLoop&&) = delete;
		CommandLoop& operator=(const CommandLoop&) = delete;
		CommandLoop& operator=(CommandLoop&&) = delete;
		std::optional<std::string> waitForCommand();
	
	private:
		// I wish this didn't have to be static, but the readline handler
		// needs to be able to see it
		static std::optional<std::string> command_received;
		void read_command_loop(); 

		boost::fibers::unbuffered_channel<bool> ready_for_command_channel;
		boost::fibers::unbuffered_channel<std::string> command_channel;
		boost::fibers::fiber read_command_loop_fiber;
		bool prompted_readline = false;

		// So that it can see the command_received static member
		friend void handle_line(char* cline);

		// So it can see if readline has been prompted
		friend class ConsoleOutputter;
};

#endif