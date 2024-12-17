#pragma once

#include <filesystem>
#include <set>
#include <sstream>
#include <string>
#include <vector>


// Check if next character is a special character
bool nextIsSpecialChar(std::string_view cmd, size_t idx) {
    std::set<char> specialChars{ '$', '`', '\\', '\n'};

    return specialChars.find(cmd.at(idx + 1)) != specialChars.end();
}

// Populates the arguments for the command
std::vector<std::string> populateArguments(std::string_view cmd)
{
    std::vector<std::string> explodedString{};

    std::string normalBuffer{};
    std::string singleQuoteBuffer{};
    std::string doubleQuoteBuffer{};

    bool singleQuoting{ false };
    bool doubleQuoting{ false };

    for (size_t i{ 0 }; i < cmd.size(); i++) {

        const auto& character{ cmd.at(i) };
 
        // If we run into a ' while not double-quoting
        if (character == '\'' && !doubleQuoting)
        {
            // Toggle single quoting mode
            singleQuoting = !singleQuoting;

            // If we're no longer single quoting, add the quote buffer to the vector
            if (!singleQuoting)
            {
                explodedString.push_back(singleQuoteBuffer);
                singleQuoteBuffer.clear();
            }

            // Move on to next char
            continue;
        }

        // If we run into a ' while double-quoting, treat it like a normal char and 
		// add it to the double-quote buffer
        else if (character == '\'' && doubleQuoting)
        {
            doubleQuoteBuffer.push_back(character);
			continue;
        }

        // If we see a ", toggle double quoting
        if (character == '"')
        {
            // Toggle double quoting mode
            doubleQuoting = !doubleQuoting;

            if (!doubleQuoting)
            {
                explodedString.push_back(doubleQuoteBuffer);
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
                explodedString.push_back(normalBuffer);
                normalBuffer.clear();
            }
        }

        // If we see any other character
        else
        {
            // If we're single-quoting, add to the quote buffer
            if (singleQuoting) singleQuoteBuffer.push_back(character);
            
            // If we're double-quoting
            else if (doubleQuoting) doubleQuoteBuffer.push_back(character);

            // If we're not single-quoting or double-quoting, add to the normal buffer
            else normalBuffer.push_back(character);
        }
    }

    // Prevent empty buffer from being written
    if (!normalBuffer.empty())
        explodedString.push_back(normalBuffer);

    return explodedString;
}

// Get the name of the current OS by using preprocessor directives
std::string getOsName()
{
    #ifdef _WIN64
        return "Windows 64-bit";
    #elif _WIN32
        return "Windows 32-bit";
    #elif __APPLE__ || __MACH__
        return "Mac OSX";
    #elif __linux__
        return "Linux";
    #elif __FreeBSD__
        return "FreeBSD";
    #elif __unix || __unix__
        return "Unix";
    #else
        return "Other";
    #endif
}

// Windows and Unix use different PATH delimiters. Return `;` for Windows and `:` for Unix
char getPATHDelim() {
    return getOsName().find("Windows") != std::string::npos ? ';' : ':';
}

// Windows and Unix use different folder separators. Return `\` for Windows and `/` for Unix
char getOSSlash() {
    return static_cast<char>(std::filesystem::path::preferred_separator);
}

// Get the user profile depending on whether we're in Windows or Linux
std::filesystem::path getUserHomeDir() {
    // Check if OS is Windows
    if (getOsName().find("Windows") != std::string::npos)
    {
        return std::filesystem::path{ getenv("USERPROFILE") };
    }

    return std::filesystem::path{ getenv("HOME") };
}

// Get the path associated with a given command, if any. Returns empty string if not
std::string getPath(const std::string& cmd) {
 
    // Read the PATH
    const auto PATH{ getenv("PATH") };

    // If PATH not found, return empty PATH vector
    if (!PATH) {
        return "";
    }

    // Get the PATH delimiter based on OS
    const auto OS_PATH_DELIM{ getPATHDelim() };

    // Get the PATH separator slash based on OS
    const auto OS_PATH_SLASH{ getOSSlash() };

    // Create a string stream to read the PATH with
    std::stringstream ss(PATH);

    // Save each delimited path in PATH here
    std::string path{};

    // Iterate through the string stream of the PATH
    while (!ss.eof()) {
        std::getline(ss, path, OS_PATH_DELIM);

        // Attach the given command to the end of the current path
        std::filesystem::path absolutePath{ path + OS_PATH_SLASH + cmd };

        // Check to see if the path exists
        if (std::filesystem::exists(absolutePath)) {
            return absolutePath.string();
        }

        // Try the above, but for Windows...

        absolutePath = path + OS_PATH_SLASH + cmd + ".exe";

        if (std::filesystem::exists(absolutePath)) {
            return absolutePath.string();
        }
    }
    return "";
}

// Runs a given command
void invokeCommand(const std::string& cmd) {
    std::system(cmd.c_str());
}

// Get the current working directory
std::string getCWD() {
    return std::filesystem::current_path().string();
}
