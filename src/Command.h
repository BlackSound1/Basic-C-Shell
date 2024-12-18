#pragma once

#include <cctype>
#include <string>
#include <utility>
#include <vector>

#include "helpers.h"


struct Command
{
	const std::string Name;
	const std::vector<std::string> Arguments;
};

std::pair<std::string, size_t> populateCommandName(std::string_view input) {

	// If the first character is a quote
	if (input[0] == '\'' || input[0] == '"')
	{
		// Toggle single and double quoting
		bool singleQuoting{ input[0] == '\'' };
		bool doubleQuoting{ input[0] == '"' };
		bool escaping{ false };

		// Create the buffers
		std::string normalBuffer{};
		std::string singleQuoteBuffer{};
		std::string doubleQuoteBuffer{};

		// Create the string to be built up
		std::string cmdName{};

		// Set the initial index
		size_t idx = 1;

		// Iterate through the input
		for (size_t i = 1; i < input.size(); i++)
		{
			idx = i;
			// Get current character
			const auto& character{ input.at(i) };

			// If we're escaping, push the char to the double-quote buffer
			if (escaping) {
				// If we're double-quoting, push the char to the double-quote buffer
				if (doubleQuoting)
					doubleQuoteBuffer.push_back(character);

				// If we're not quoting at all, push the char to the normal buffer
				if (!doubleQuoting && !singleQuoting)
					normalBuffer.push_back(character);

				escaping = false;
				continue;
			}

			// If we run into a ' while not double-quoting
			if (character == '\'' && !doubleQuoting)
			{
				// Toggle single quoting mode
				singleQuoting = !singleQuoting;

				// If we're no longer single quoting, add the quote buffer to the vector
				if (!singleQuoting)
				{
					cmdName += singleQuoteBuffer;
					singleQuoteBuffer.clear();
				}

				// Move on to next char
				continue;
			}

			// If we run into a ' while double-quoting, treat it like a normal char and
			// add it to the double-quote buffer
			if (character == '\'' && doubleQuoting)
			{
				doubleQuoteBuffer.push_back(character);
				continue;
			}

			// Escaping is activated when not quoting or when double quoting,
			// but only if followed by char in [\, $, ", \n]
			if (
				character == '\\' &&
				(
					(!singleQuoting && !doubleQuoting) ||
					(doubleQuoting && nextIsSpecialChar(input, i))
				)
			)
			{
				escaping = true;
				continue;
			}

			// If we see a "...
			if (character == '"')
			{
				// If we're double quoting, but the next char is a non-space, skip this char
				if (doubleQuoting && (i + 1) < input.size() && !std::isspace(input.at(i + 1))) continue;

				// If we're currently single-quoting, treat this as a normal char
				if (singleQuoting)
				{
					singleQuoteBuffer.push_back(character);
					continue;
				}

				// Otherwise, toggle double-quoting mode
				doubleQuoting = !doubleQuoting;

				// If we're no longer double-quoting, add the buffer to the vector
				if (!doubleQuoting)
				{
					cmdName += doubleQuoteBuffer;
					doubleQuoteBuffer.clear();
				}

				continue;
			}

			// If we see a space
			if (character == ' ')
			{
				// If we're single quoting, add the space to the quote buffer
				if (singleQuoting) singleQuoteBuffer.push_back(character);

				// If we're double quoting, add the space to the double quote buffer
				else if (doubleQuoting) doubleQuoteBuffer.push_back(character);

				// If we're not quoting, dump the normal buffer,
				// assuming the normal buffer is not empty
				else if (!normalBuffer.empty())
				{
					cmdName += normalBuffer;
					normalBuffer.clear();
				}

				continue;
			}

			// If we see any other character...

			// If we're single-quoting, add to the single quote buffer
			if (singleQuoting)
			{
				singleQuoteBuffer.push_back(character);
				continue;
			}

			// If we're double-quoting, add to the double quote buffer
			if (doubleQuoting)
			{
				doubleQuoteBuffer.push_back(character);
				continue;
			}

			// Add to the normal buffer, if all else fails
			normalBuffer.push_back(character);
		}

		// If the double buffer is not empty, push it
		if (!doubleQuoteBuffer.empty())
			cmdName += doubleQuoteBuffer;

		// If the normal buffer is not empty, push it
		if (!normalBuffer.empty())
			cmdName += normalBuffer;

		return { cmdName, idx };
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

	//std::string cmdName{ name };
	//std::vector<std::string> cmdArgs{ populateArguments(std::string_view(input.substr(idx + 1))) };

	//// Get the command name by first getting the index of the first space.
	//if (const auto& idx{ input.find(' ') }; idx == std::string::npos)
	//{
	//	// If there is no space found
	//	cmdName = input;
	//}
	//else
	//{
	//	// Get the substring of the first word until the first space
	//	cmdName = input.substr(0, idx);

	//	// Get the substring of everything after the first word
	//	cmdArgs = populateArguments(static_cast<std::string>(input.substr(idx + 1)));
	//}

	return { name, populateArguments(std::string_view(input.substr(idx + 1))) };

	//return { cmdName, cmdArgs };

	//return { populateCommandName(input), populateArguments(std::string{input.substr(idx + 1)} };

	//return { cmdName, std::vector<std::string> {cmdArgs} };
}
