// ---------------------------------------------------------
// LEVEL 3: The Adventure Begins (Control Flow)
// ---------------------------------------------------------
// Games aren't much fun if the same thing happens every time.
// We use Control Flow (if/else and loops) to make decisions
// and repeat actions.

#include <iostream>
#include <string>

int main() {
    std::cout << "You stand at a fork in the dark dungeon." << std::endl;
    std::cout << "Do you go 'left' or 'right'? ";
    
    std::string choice;
    std::cin >> choice;

    // 1. IF / ELSE IF / ELSE Statements
    // The computer will evaluate the condition inside the parenthesis ( )
    // If it's true, it runs the code inside the curly braces { }
    if (choice == "left") {
        std::cout << "\nYou found a treasure chest! +100 Gold!" << std::endl;
    } 
    else if (choice == "right") {
        std::cout << "\nOh no! A wild Goblin appears!" << std::endl;
        
        // Let's set up a battle!
        int goblinHealth = 30;
        int playerDamage = 12;
        int round = 1;

        // 2. WHILE Loops
        // This code will repeat AS LONG AS the condition is true.
        while (goblinHealth > 0) {
            std::cout << "\n--- Round " << round << " ---" << std::endl;
            std::cout << "You strike the Goblin for " << playerDamage << " damage!" << std::endl;
            
            // Subtract playerDamage from goblinHealth
            goblinHealth = goblinHealth - playerDamage;
            
            std::cout << "Goblin HP is now: " << goblinHealth << std::endl;
            round = round + 1;
        }

        std::cout << "\nThe Goblin is defeated! You win!" << std::endl;
    } 
    else {
        // This runs if none of the above conditions were true
        std::cout << "\nYou stumble around in the dark and bump your head." << std::endl;
    }

    std::cout << "Thanks for playing!" << std::endl;

    return 0;
}
