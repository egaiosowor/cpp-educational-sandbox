// ---------------------------------------------------------
// LEVEL 2: Welcome to the Tavern (Variables & I/O)
// ---------------------------------------------------------
// Now that we know about data types, let's learn how to
// interact with the player using variables!
// A variable is like a named box in memory where we store data.

#include <iostream>
#include <string> // We need this to use words and sentences (strings)

int main() {
    // std::string is a sequence of characters (text).
    // Note: strings use double quotes!
    std::string tavernName = "The Prancing Pony";
    
    // We can also create a variable but leave it empty for now.
    std::string playerName;
    int playerAge;

    std::cout << "Welcome to " << tavernName << "!" << std::endl;
    std::cout << "What is your name, traveler? ";

    // std::cin stands for "character input". It waits for the 
    // player to type something and press Enter, then stores it in the variable.
    std::cin >> playerName;

    std::cout << "Ah, " << playerName << "! How old are you? ";
    std::cin >> playerAge;

    // We can use variables in mathematical expressions!
    int daysAlive = playerAge * 365;

    std::cout << "\n--- TAVERN KEEPER SAYS ---" << std::endl;
    std::cout << "Nice to meet you, " << playerName << "." << std::endl;
    std::cout << "You have been alive for roughly " << daysAlive << " days!" << std::endl;
    std::cout << "Have a drink on the house!" << std::endl;

    return 0;
}
