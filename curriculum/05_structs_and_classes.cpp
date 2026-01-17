// ---------------------------------------------------------
// LEVEL 5: Grouping Data (Structs and Classes)
// ---------------------------------------------------------
// Passing around individual variables for health, name, damage
// gets confusing. We can use Objects to group related data
// together!

#include <iostream>
#include <string>

// A struct is a simple way to group data.
// Everything inside is public by default.
struct Weapon {
    std::string name;
    int damageBonus;
};

// A class is like a blueprint for an object.
// It can have both data (variables) and behaviors (functions).
class Player {
private:
    // Private data cannot be accessed directly from outside the class.
    // This keeps our data safe from accidental changes!
    std::string name;
    int health;
    int maxHealth;

public:
    // A Constructor is a special function that runs when the object is created.
    Player(std::string startingName, int startingHP) {
        name = startingName;
        maxHealth = startingHP;
        health = startingHP; // Start at max health
    }

    // Public functions (methods) allow outside code to interact with the object.
    void takeDamage(int amount) {
        health = health - amount;
        if (health < 0) health = 0;
        std::cout << name << " takes " << amount << " damage! (HP: " << health << ")" << std::endl;
    }

    void heal(int amount) {
        health = health + amount;
        if (health > maxHealth) health = maxHealth;
        std::cout << name << " heals " << amount << " HP! (HP: " << health << ")" << std::endl;
    }

    // A "getter" function lets us read private data safely.
    std::string getName() {
        return name;
    }
};

int main() {
    // 1. Using a struct
    Weapon mySword;
    mySword.name = "Rusty Iron Sword";
    mySword.damageBonus = 3;
    
    std::cout << "You found a " << mySword.name << " (+ " << mySword.damageBonus << " DMG)" << std::endl;

    // 2. Using a class
    // We create an instance (object) of the Player class.
    Player hero("Arthur", 100);

    std::cout << "\nOur hero " << hero.getName() << " enters the fray!" << std::endl;

    // We interact with the object using its public methods.
    hero.takeDamage(25);
    hero.heal(10);
    hero.takeDamage(100); // Uh oh!

    return 0;
}
