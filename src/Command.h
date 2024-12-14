#pragma once

#include <string>
#include <vector>
#include "helpers.h"


struct Command
{
	const std::string Name;
	const std::vector<std::string> Arguments;
};

// Parses a given input into a Command
Command parseCommand(std::string_view input) {

	std::string cmdName{};
	std::vector<std::string> cmdArgs{};
	
	// Get the command name by first getting the index of the first space.
	if (const auto& idx{ input.find(' ') }; idx == std::string::npos)
	{
		// If there is no space found
		cmdName = input;
	}
	else
	{
		// Get the substring of the first word until the first space
		cmdName = input.substr(0, idx);

		// Get the substring of everything after the first word
		cmdArgs = populateArguments(static_cast<std::string>(input.substr(idx)));
	}

	return { cmdName, std::vector<std::string> {cmdArgs} };
}
