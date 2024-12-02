#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

#include "helpers.h"

bool isValidCommand(const std::string &cmd);
std::vector<std::string> explodeString(const std::string &cmd);
int runCommand(const std::string& cmd);

int main() {
	// Flush after every std::cout / std:cerr
	std::cout << std::unitbuf;
	std::cerr << std::unitbuf;

	int exitCode = 0;
	std::string input;

	while (true) {
		std::cout << "$ ";

		std::getline(std::cin, input);

		bool isValid = isValidCommand(input);

		if (isValid) {
			exitCode = runCommand(input);

			if (exitCode == 0){
				return 0;
			}
			else if (exitCode == -1){
				continue;
			}
			else{
				return exitCode;
			}
		}
		else {
			std::cout << input << ": command not found" << std::endl;
		}
	}

   return exitCode;
}

int runCommand(const std::string& cmd) {
	// Get the exploded command
	auto explodedCommand = explodeString(cmd);
	std::string cmdName = explodedCommand[0];
	
	if (cmdName == "exit"){
		// If we call exit, must have an exit code
		if (explodedCommand.size() != 2){
			return -1;
		}

		// Get exit code
		int exitCode = std::atoi(explodedCommand[1].c_str());

		return exitCode;
	}
	else if (cmdName == "echo"){
		std::vector<std::string> whatToPrintVec{};

		for (std::vector<std::string>::iterator vecIterator = explodedCommand.begin() + 1; vecIterator != explodedCommand.end(); vecIterator++){
			whatToPrintVec.push_back(*vecIterator);
		}

		std::ostringstream concatted{};
		const char* const delim = " ";
		std::copy(whatToPrintVec.begin(), whatToPrintVec.end(), std::ostream_iterator<std::string>(concatted, delim));

		std::string hey = concatted.str();

		auto concattedStr = trim(hey);

		std::cout << concattedStr << std::endl;

		return -1;
	}
	else{
		return 0;
	}
}

// Checks if a command is valid
bool isValidCommand(const std::string &cmd) {
	
	// Split command into tokens
	auto explodedCommand = explodeString(cmd);

	// Get the command name
	std::string cmdName = explodedCommand[0];

	if (cmdName == "exit") {
		return true;
	}
	else if (cmdName == "echo")
	{
		return true;
	}
	else {
		return false;
	}
}

std::vector<std::string> explodeString(const std::string& cmd)
{
	std::vector<std::string> explodedString;
	std::stringstream ss(cmd);

	std::string token;
	while (std::getline(ss, token, ' ')) {
		explodedString.push_back(token);
	}

	return explodedString;
}
