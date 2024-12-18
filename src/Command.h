#pragma once

#include <cctype>
#include <string>
#include <utility>
#include <vector>

#include "helpers.h"


// A Command has a name and 0 or more arguments
struct Command
{
	const std::string Name;
	const std::vector<std::string> Arguments;
};

std::pair<std::string, size_t> _handleQuotedCmdName(std::string_view input) {
	bool inSingleQuote{ input[0] == '\'' };
	bool inDoubleQuote{ input[0] == '"' };
	bool escaping{ false };

	std::string cmdName{};

	for (size_t i{ 1 }; i < input.size(); i++)
	{
		const auto& character{ input[i] };

		// If we're escaping, add the character
		if (escaping) {
			cmdName += character;
			escaping = false;
			continue;
		}

		// If the current character is a space and we're not quoting at all,
		// We have reached the first space not in the quotes, so return
		if (character == ' ' && !inSingleQuote && !inDoubleQuote)
		{
			return { cmdName, i };
		}

		// If we're single-quoting and we see a ', stop single-quoting
		if (character == '\'' && inSingleQuote)
		{
			inSingleQuote = false;
			continue;
		}

		// If we're double-quoting and we see a ", stop double-quoting
		if (character == '"' && inDoubleQuote)
		{
			inDoubleQuote = false;
			continue;
		}

		// Escaping is activated when double quoting,
		// but only if followed by char in [\, $, ", \n]
		if (character == '\\' && inDoubleQuote && nextIsSpecialChar(input, i))
		{
			escaping = true;
			continue;
		}

		// If we are not escaping and did not see a quote, add the character
		cmdName += character;
	}
}

// Returns a pair of the command name and the index of the first, non-quoted space
std::pair<std::string, size_t> populateCommandName(std::string_view input) {

	// If the first character is a quote
	if (input[0] == '\'' || input[0] == '"')
	{
		return _handleQuotedCmdName(input);
	}

	// If the first character is not a quote
	else
	{
		// Try to find the first space
		const auto& idx{ input.find(' ') };

		// If no space found, just return the input and -1
		if (idx == std::string::npos)
			return { std::string{ input }, -1 };

		// If there is a space, return the substring of the first word until the first space
		// and the index of the space
		return { std::string{ input.substr(0, idx) }, idx };
	}
}

// Parses a given input into a Command
Command parseCommand(std::string_view input) {

	const auto& [name, idx] { populateCommandName(input) };

	return { name, populateArguments(std::string_view(input.substr(idx + 1))) };
}
