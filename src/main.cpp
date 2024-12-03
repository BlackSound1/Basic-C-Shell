#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#include "helpers.h"

const std::vector<std::string> builtins{"exit", "echo", "type", "pwd"};

int runCommand(const std::string &cmd);

int main() {
	// Flush after every std::cout / std:cerr
	std::cout << std::unitbuf;
	std::cerr << std::unitbuf;

	int exitCode = 0;
	std::string input;

	while (true) {
		std::cout << "$ ";

		std::getline(std::cin, input);

		exitCode = runCommand(input);

		if (exitCode == -1) {
			continue;
		}
		else if (exitCode == -2) {
			std::cout << input << ": command not found" << std::endl;
		}
		else {
			return exitCode;
		}
	}

   return exitCode;
}

int runCommand(const std::string &cmd) {
	// Get the exploded command
	auto explodedCommand = explodeString(cmd);
	std::string cmdName = explodedCommand[0];
	
	if (cmdName == "exit") {
		// If we call exit, must have an exit code
		if (explodedCommand.size() != 2){
			return -1;
		}

		// Get exit code
		int exitCode = std::atoi(explodedCommand[1].c_str());

		return exitCode;
	}
	else if (cmdName == "echo") {
		
		const int ECHO_LENGTH = 5; // Including the space
		std::string textToEcho = cmd.substr(ECHO_LENGTH);

		std::cout << textToEcho << std::endl;

		return -1;
	}
	else if (cmdName == "type") {
		// Make sure this command has exactly 1 argument
		if (explodedCommand.size() != 2) {
			std::cout << "type command needs exactly one argument" << std::endl;
			return -1;
		}

		// Get the argument
		auto const& arg{ explodedCommand[1] };

		// If it's a builtin, return
		if (vecContains(arg, builtins)) {
			std::cout << arg << " is a shell builtin" << std::endl;
			return -1;
		}

		// Try to get the path for the given command, if any
		const auto& path{ getPath(arg) };
		
		// If we found the program, return
		if (!path.empty()) {
			std::cout << arg << " is " << path << std::endl;

			return -1;
		}

		// If we didn't find it, return
		std::cout << arg << ": not found" << std::endl;
		
		return -1;
	}
	else if (cmdName == "pwd") {
		std::cout << getCWD() << std::endl;

		return -1;
	}
	else {
		
		// Try to get the absolute path of the command
		const auto& pathToCmd{ getPath(cmdName) };

		// If we found nothing, return early
		if (pathToCmd.empty()) {
			return -2;
		}

		// Try to run the command. If we get some positive, non-zero status, return it
		if (const auto status{ invokeCommand(cmd) }; status > 0) {
			return status;
		}
		else {
			return -1;
		}

		// Else, just return -2 so we can say the command wasn't found
		return -2;
	}
}
