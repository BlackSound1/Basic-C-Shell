#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#include "helpers.h"
#include "Builtins.h"
#include "Command.h"


int main() {

	// Flush after every std::cout / std:cerr
	std::cout << std::unitbuf;
	std::cerr << std::unitbuf;

	int exitCode{ 0 };
	std::string input{};

	while (true) {
		std::cout << "$ ";

		std::getline(std::cin, input);

		const Command cmd{ parseCommand(input) };

		const auto& cmdName{ cmd.Name };
		const auto& cmdArgs{ cmd.Arguments };

		if (cmdName == "exit")
		{
			return handleExitCommand(cmdArgs);
		}
		else if (cmdName == "echo")
		{
			handleEchoCommand(cmdArgs);
		}
		else if (cmdName == "type")
		{
			handleTypeCommand(cmdArgs);
		}
		else if (cmdName == "pwd")
		{
			std::cout << getCWD() << std::endl;
		}
		else if (cmdName == "cd") {
			handleCdCommand(cmdArgs);
		}
		else {
			handleUnknownCommand(cmdName, input);
		}
	}

   return exitCode;
}
