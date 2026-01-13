// ---------------------------------------------------------
// LEVEL 1: Building Blocks (Computing Basics & Data Types)
// ---------------------------------------------------------
// Welcome to C++! 
// A computer program is just a list of instructions we give
// to the computer. Before the computer can run these instructions,
// we have to "compile" them - translating human-readable code
// into machine code (1s and 0s).
//
// In this lesson, we look at Data Types: the different ways a
// computer can store information in memory.

#include <iostream>

int main() {
    // 1. Integer (int) - Whole numbers (no decimals)
    // We use this for things like Max Health or Level.
    int maxHealth = 100;
    int currentLevel = 1;

    // 2. Floating-point (float, double) - Numbers with decimals
    // We use this for things like currency or precise distances.
    float goldCoins = 5.50f; 
    double preciseDistance = 1234.56789;

    // 3. Character (char) - A single letter or symbol
    // We use this for grades, single key presses, etc.
    // Note: chars use single quotes!
    char playerGrade = 'S';

    // 4. Boolean (bool) - True or False
    // We use this for simple yes/no states.
    bool isAlive = true;
    bool hasMagicSword = false;

    // The code below just prints our data to the screen so we can see it!
    // std::cout stands for "character output"
    // std::endl means "end line" (move to the next line)
    std::cout << "--- CHARACTER STATS ---" << std::endl;
    std::cout << "Max Health: " << maxHealth << std::endl;
    std::cout << "Level:      " << currentLevel << std::endl;
    std::cout << "Gold:       " << goldCoins << std::endl;
    std::cout << "Grade:      " << playerGrade << std::endl;
    
    std::cout << "Is Alive?   " << isAlive << std::endl; 
    // Notice that 'true' prints as a '1' in C++ by default!

    return 0; // This tells the computer our program finished successfully
}
