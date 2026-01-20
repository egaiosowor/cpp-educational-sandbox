// ---------------------------------------------------------
// LEVEL 8: The Bestiary (Inheritance)
// ---------------------------------------------------------
// Inheritance allows us to create new classes based on existing ones.
// We can have a base 'Enemy' class, and derive specific types of
// enemies from it, like 'Goblin' or 'Dragon'.

#include <iostream>
#include <string>

// 1. The Base Class (Parent)
class Enemy {
protected: 
    // protected is like private, but allows derived classes to access these variables.
    std::string name;
    int health;

public:
    Enemy(std::string n, int hp) : name(n), health(hp) {}

    void takeDamage(int amount) {
        health -= amount;
        std::cout << name << " took " << amount << " dmg! (HP: " << health << ")" << std::endl;
    }

    // A normal function
    void taunt() {
        std::cout << name << " growls at you!" << std::endl;
    }
};

// 2. The Derived Class (Child)
// The ": public Enemy" means Goblin INHERITS from Enemy.
class Goblin : public Enemy {
public:
    // We must call the Parent's constructor in our initialization list
    Goblin() : Enemy("Sneaky Goblin", 30) {}

    // Goblin can have its own unique functions too!
    void stealGold() {
        std::cout << name << " tries to steal your gold!" << std::endl;
    }
};

class Dragon : public Enemy {
public:
    Dragon() : Enemy("Fire Dragon", 200) {}

    void breathFire() {
        std::cout << name << " breathes a massive cone of fire!" << std::endl;
    }
};

int main() {
    Goblin myGoblin;
    Dragon myDragon;

    // Both inherit the taunt() and takeDamage() methods from Enemy!
    myGoblin.taunt();
    myDragon.taunt();

    myGoblin.takeDamage(10);
    myDragon.takeDamage(25);

    std::cout << std::endl;

    // But they also have their own unique abilities:
    myGoblin.stealGold();
    myDragon.breathFire();

    return 0;
}
