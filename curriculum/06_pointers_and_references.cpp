// ---------------------------------------------------------
// LEVEL 6: The Magic of Memory (Pointers & References)
// ---------------------------------------------------------
// In C++, variables live in specific locations in memory.
// Pointers and References let us talk about WHERE the data
// is, rather than just copying it.

#include <iostream>
#include <string>

// 1. Pass by Value (makes a copy)
// This function gets a COPY of the health. Modifying it doesn't change the original.
void tryToHeal(int hpCopy) {
    hpCopy = 100;
}

// 2. Pass by Reference (uses the original)
// The '&' means we are passing the ACTUAL variable, not a copy.
void actualHeal(int& realHP) {
    realHP = 100;
}

int main() {
    int playerHealth = 10;
    
    std::cout << "Starting HP: " << playerHealth << std::endl;

    tryToHeal(playerHealth);
    std::cout << "After tryToHeal: " << playerHealth << " (Still 10!)" << std::endl;

    actualHeal(playerHealth);
    std::cout << "After actualHeal: " << playerHealth << " (Fully healed!)" << std::endl;

    std::cout << "\n-------------------\n" << std::endl;

    // 3. Pointers
    // A pointer is a variable that stores a MEMORY ADDRESS.
    int gold = 50;
    int* goldPointer = &gold; // The '&' here means "Address of"

    std::cout << "Gold amount: " << gold << std::endl;
    std::cout << "Where gold lives in memory: " << goldPointer << std::endl;

    // We can use the '*' to "dereference" the pointer, 
    // which means "go to the address and get the value".
    std::cout << "Value at the pointer's address: " << *goldPointer << std::endl;

    // If we change the value AT the address, the original variable changes!
    *goldPointer = 999;
    std::cout << "We changed the value via pointer!" << std::endl;
    std::cout << "New Gold amount: " << gold << std::endl;

    // Why use pointers? 
    // In games, we use them for things like dynamic memory allocation (using 'new')
    // to spawn enemies or loot while the game is running, and then we must
    // 'delete' them when they die to prevent memory leaks!
    
    std::string* dynamicLoot = new std::string("Epic Shield");
    std::cout << "\nYou found loot: " << *dynamicLoot << std::endl;
    
    delete dynamicLoot; // Always clean up what you 'new'!

    return 0;
}
