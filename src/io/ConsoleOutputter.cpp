#include "ConsoleOutputter.h"

#include "CommandLoop.h"

#include <iostream>
#include <readline.h>

void ConsoleOutputter::write(const std::string_view& string) {
	bool prompted_readline = commandLoop.prompted_readline;
	if(prompted_readline) {
		rl_clear_visible_line();
	}
	// TODO check for trailing newline
	output << string << "\n";
	if(prompted_readline) {
		rl_on_new_line();
		rl_redisplay();
	}
}