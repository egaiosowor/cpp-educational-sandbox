// ---------------------------------------------------------
// LEVEL 4: Learning Skills (Functions)
// ---------------------------------------------------------
// As our game gets bigger, writing all our code inside main()
// gets very messy. We can use Functions to bundle up blocks
// of code so we can reuse them easily.

#include <iostream>
#include <string>

// 1. A simple function that returns nothing (void)
// It just prints a menu.
void printMenu() {
    std::cout << "\n--- ACTIONS ---" << std::endl;
    std::cout << "1. Attack" << std::endl;
    std::cout << "2. Heal" << std::endl;
    std::cout << "3. Run Away" << std::endl;
    std::cout << "---------------" << std::endl;
}

// 2. A function that takes arguments and returns a value (int)
// This calculates how much damage an attack should do.
int calculateDamage(int baseDamage, int weaponBonus) {
    int totalDamage = baseDamage + weaponBonus;
    return totalDamage;
}

// 3. A function to handle healing
// It takes current health, max health, and heal amount, returning new health.
int healPlayer(int currentHP, int maxHP, int healAmount) {
    int newHP = currentHP + healAmount;
    if (newHP > maxHP) {
        newHP = maxHP; // Can't heal past max!
    }
    std::cout << "You drink a potion and recover " << healAmount << " HP." << std::endl;
    return newHP;
}

int main() {
    std::cout << "You are facing a grumpy Troll!" << std::endl;
    
    int playerHP = 20;
    int playerMaxHP = 50;
    
    // Call the function to print the menu
    printMenu();

    int choice;
    std::cout << "What do you do? ";
    std::cin >> choice;

    if (choice == 1) {
        // Call the function to calculate damage
        int damage = calculateDamage(10, 5); // 10 base, 5 from weapon
        std::cout << "You dealt " << damage << " damage to the Troll!" << std::endl;
    } 
    else if (choice == 2) {
        // Call the healing function and update our health
        playerHP = healPlayer(playerHP, playerMaxHP, 15);
        std::cout << "Your HP is now: " << playerHP << "/" << playerMaxHP << std::endl;
    } 
    else {
        std::cout << "You run away like a coward!" << std::endl;
    }

    return 0;
}
