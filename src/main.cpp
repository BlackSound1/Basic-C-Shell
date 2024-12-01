#include <iostream>
#include <string>
#include <sstream>

bool isValidCommand(const std::string &cmd);

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

   std::cout << "$ ";
  
   std::string input;
   std::getline(std::cin, input);

   bool isValid = isValidCommand(input);

   if (isValid) {
	   std::cout << "" << std::endl;
   }
   else {
	   std::cout << input << ": command not found" << std::endl;
   }

   return 0;

}

// Checks if a command is valid
bool isValidCommand(const std::string &cmd) {
	return false;
}