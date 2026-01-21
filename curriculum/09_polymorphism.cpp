// ---------------------------------------------------------
// LEVEL 9: Epic Encounters (Polymorphism)
// ---------------------------------------------------------
// Polymorphism means "many forms". It allows us to treat different
// derived classes as if they were the base class, and let C++ figure
// out which specific function to call at runtime!

#include <iostream>
#include <string>
#include <vector>

class Enemy {
protected:
    std::string name;
public:
    Enemy(std::string n) : name(n) {}
    
    // A 'virtual' function means "I can be overridden by my children."
    virtual void attack() {
        std::cout << name << " does a basic attack!" << std::endl;
    }

    // A virtual destructor is critical when using inheritance and pointers!
    virtual ~Enemy() {}
};

class Goblin : public Enemy {
public:
    Goblin() : Enemy("Goblin") {}

    // We use the 'override' keyword to explicitly state we are overriding the parent.
    void attack() override {
        std::cout << name << " attacks you with a rusty dagger! *Stab stab*" << std::endl;
    }
};

class Dragon : public Enemy {
public:
    Dragon() : Enemy("Dragon") {}

    void attack() override {
        std::cout << name << " roars and breathes fire! *Fwoosh*" << std::endl;
    }
};

class Skeleton : public Enemy {
public:
    Skeleton() : Enemy("Skeleton") {}

    void attack() override {
        std::cout << name << " shoots an arrow! *Twang*" << std::endl;
    }
};

int main() {
    // By storing POINTERS to the base class (Enemy*) in our vector,
    // we can store any derived class (Goblin, Dragon, Skeleton) inside it!
    std::vector<Enemy*> enemies;

    // We create them dynamically using 'new'
    enemies.push_back(new Goblin());
    enemies.push_back(new Skeleton());
    enemies.push_back(new Dragon());

    std::cout << "--- ENEMY TURN ---" << std::endl;

    // We iterate through all enemies
    for (Enemy* e : enemies) {
        // Here is the magic of Polymorphism!
        // Even though the pointer is an Enemy*, C++ knows exactly
        // which real object it is pointing to and calls the correct attack().
        e->attack();
    }

    // Clean up memory
    for (Enemy* e : enemies) {
        delete e; 
    }
    enemies.clear();

    return 0;
}
