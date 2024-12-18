#pragma once

#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "helpers.h"

const std::vector<std::string> builtins{ "exit", "echo", "type", "pwd", "cd" };

bool isBuiltin(const std::string& cmd) {
	if (std::find(builtins.begin(), builtins.end(), cmd) != builtins.end())
	{
		return true;
	}
	return false;
}

// Handles the exit builtin command
int handleExitCommand(std::vector<std::string> args) {

	// If no argument provided, assume no error
	if (args.empty()) {
		return 0;
	}

	// If too many args provided, exit with error
	if (args.size() > 1)
	{
		std::cout << "Too many arguments for exit command. Exiting with error" << std::endl;
		return 1;
	}

	// Just return the given exit code
	return std::atoi(args[0].c_str());
}

// Handles the echo builtin command
void handleEchoCommand(std::vector<std::string> args) {
	for (size_t i = 0; i < args.size(); i++)
	{
		if (i == args.size() - 1)
			std::cout << args.at(i);
		else
			std::cout << args.at(i) << " ";
	}

	std::cout << std::endl;
}

// Handles the type builtin command
void handleTypeCommand(std::vector<std::string> args) {

	// Make sure this command has exactly 1 argument
	if (args.size() != 1) {
		std::cout << "type command needs exactly one argument" << std::endl;
		return;
	}

	// Get the argument
	const auto& argument{ args.at(0) };

	// If it's a builtin...
	if (isBuiltin(argument)) {
		std::cout << argument << " is a shell builtin" << std::endl;
		return;
	}

	// Try to get the path for the given command, if any
	const auto& path{ getPath(argument) };

	// If we found the program...
	if (!path.empty()) {
		std::cout << argument << " is " << path << std::endl;
		return;
	}

	// If we didn't find it...
	std::cout << argument << ": not found" << std::endl;
}

// Handles the cd builtin command
void handleCdCommand(std::vector<std::string> args) {

	// For now, if cd is run by itself, just do nothing
	if (args.empty()) {
		return;
	}

	// Get the directory to return to
	const std::filesystem::path& pathString{ args[0] };

	// Handle home directory
	if (pathString.string() == "~") {
		const auto& HOME_DIR{ getUserHomeDir() };

		// Convert it to path
		const auto& path{ std::filesystem::path(HOME_DIR) };

		// Set the cwd to the given absolute path
		std::filesystem::current_path(path);

		return;
	}

	// Convert it to a path
	const auto& path{ std::filesystem::path(pathString) };

	// Check if path exists
	if (!std::filesystem::exists(path)) {
		std::cerr << "cd: " << path.string() << ": No such file or directory" << std::endl;
		return;
	}

	// Set the cwd to the given absolute path
	std::filesystem::current_path(path);
}

// Handles any other unknown command. Tries to find the command on the PATH. Runs it if found
void handleUnknownCommand(std::string_view cmdName, std::string_view input) {

	// Try to get the absolute path of the command, and if we found nothing, loop
	if (getPath(std::string(cmdName)).empty()) {
		std::cout << cmdName << ": command not found" << std::endl;
		return;
	}

	invokeCommand(std::string(input));
}
