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

	void write(const std::string_view& string);

private:
	std::ostream& output;
	CommandLoop& commandLoop;
};

#endif