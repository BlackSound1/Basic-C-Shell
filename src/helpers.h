#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <filesystem>
#include <algorithm>

const char* ws = " \t\n\r\f\v";

// trim from end of string (right)
inline std::string& rtrim(std::string& s, const char* t = ws)
{
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

// trim from beginning of string (left)
inline std::string& ltrim(std::string& s, const char* t = ws)
{
    s.erase(0, s.find_first_not_of(t));
    return s;
}

// trim from both ends of string (right then left)
inline std::string& trim(std::string& s, const char* t = ws)
{
    return ltrim(rtrim(s, t), t);
}

// Populates the arguments for the command
std::vector<std::string> populateArguments(const std::string& cmd)
{
    std::vector<std::string> explodedString{};

    std::string normalBuffer{};
    std::string quoteBuffer{};

    bool singleQuoting{ false };

    for (const auto& character : cmd) {
        
        // If we run into a '
        if (character == '\'')
        {
            // Toggle single quoting mode
            singleQuoting = !singleQuoting;

            // If we're no longer single quoting, add the quote buffer to the vector
            if (!singleQuoting)
            {
                explodedString.push_back(quoteBuffer);
                quoteBuffer.clear();
            }

            // Move on to next char
            continue;
        }

        if (character == ' ')
        {
            // If we're single quoting, add the space to the quote buffer
            if (singleQuoting)
            {
                quoteBuffer.push_back(character);
                continue;
            }

            // If we're not single quoting, dump the normal buffer,
            // assuming the normal buffer is not empty
            if (!normalBuffer.empty())
            {
                explodedString.push_back(normalBuffer);
                normalBuffer.clear();
            }
        }
        else
        {
            // If we're single quoting, add to the quote buffer
            if (singleQuoting)
                quoteBuffer.push_back(character);

            // If we're not single quoting, add to the normal buffer
            else
                normalBuffer.push_back(character);
        }
    }

    explodedString.push_back(normalBuffer);

    return explodedString;
}

// Explodes a string into a vector of strings
std::vector<std::string> explodeString(const std::string& cmd, const char& delim=' ')
{
    std::vector<std::string> explodedString{};
	std::stringstream ss(cmd);

	std::string token;

    // Loop through each line of the string, separated by delim
	while (std::getline(ss, token, delim)) {

        // Add to the vector if it's not a blank string
        if (token != "")
        {
		    explodedString.push_back(token);
        }
	}

	return explodedString;
}

// Checks a given vector for a given element. Returns whether the vector contains it
bool vecContains(const std::string& str, const std::vector<std::string>& vec) {
	auto it = std::find(vec.begin(), vec.end(), str);

	return it != vec.end();
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
    
    // Check if OS is Windows
    if (const auto OS_NAME{ getOsName() }; OS_NAME.find("Windows") != std::string::npos)
    {
        return ';';
    }
    
    return ':';
}

// Windows and Unix use different folder separators. Return `\` for Windows and `/` for Unix
char getOSSlash() {
    return static_cast<char>(std::filesystem::path::preferred_separator);
}

// Get the user profile depending on whether we're in Windows or Linux
std::filesystem::path getUserHomeDir() {
    // Check if OS is Windows
    if (const auto OS_NAME{ getOsName() }; OS_NAME.find("Windows") != std::string::npos)
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
