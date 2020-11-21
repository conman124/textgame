#ifndef CONSOLEOUTPUTTER_H
#define CONSOLEOUTPUTTER_H

#include "CommandLoop.h"

#include <fmt/core.h>
#include <iostream>
#include <memory>
#include <readline.h>

class ConsoleOutputter {
public:
	ConsoleOutputter(std::ostream& outputIn, CommandLoop& commandLoopIn)
		: output{outputIn}
		, commandLoop{commandLoopIn}
	{};

	ConsoleOutputter(const ConsoleOutputter&) = delete;
	ConsoleOutputter(ConsoleOutputter&&) = delete;
	ConsoleOutputter& operator=(const ConsoleOutputter&) = delete;
	ConsoleOutputter& operator=(ConsoleOutputter&&) = delete;
	~ConsoleOutputter() = default;

	template<class... Args>
	void write(const std::string_view& fmtstr, const Args&... args);

private:
	std::ostream& output;
	CommandLoop& commandLoop;
};

template<class... Args>
void ConsoleOutputter::write(const std::string_view& fmtstr, const Args&... args) {
	bool prompted_readline = commandLoop.prompted_readline;
	if(prompted_readline) {
		rl_clear_visible_line();
	}
	output << fmt::format(fmtstr, args...) << "\n";
	if(prompted_readline) {
		rl_on_new_line();
		rl_redisplay();
	}
}

#endif