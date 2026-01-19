// ---------------------------------------------------------
// LEVEL 7: The Adventurer's Backpack (Vectors)
// ---------------------------------------------------------
// Often we need to store many items of the same type, like
// an inventory full of weapons.
// std::vector is a "dynamic array" that can grow and shrink!

#include <iostream>
#include <string>
#include <vector> // Required for vectors

int main() {
    // We create a vector that holds strings.
    // The <string> part is called a template argument.
    std::vector<std::string> inventory;

    // We can add items to the back of the vector using push_back()
    inventory.push_back("Rusty Sword");
    inventory.push_back("Health Potion");
    inventory.push_back("Torch");

    std::cout << "You open your backpack." << std::endl;
    std::cout << "It contains " << inventory.size() << " items:" << std::endl;

    // We use a for loop to iterate over the vector.
    // size_t is just a fancy unsigned integer type used for sizes.
    for (size_t i = 0; i < inventory.size(); i++) {
        // We access elements using [ ] just like a regular array.
        std::cout << "- " << inventory[i] << std::endl;
    }

    std::cout << "\nYou drink the Health Potion!" << std::endl;
    
    // We can remove items. Here we remove the 2nd item (index 1).
    // .begin() gives us an iterator to the start, and + 1 moves it forward.
    inventory.erase(inventory.begin() + 1);

    std::cout << "You found a Magic Wand!" << std::endl;
    inventory.push_back("Magic Wand");

    std::cout << "\nUpdated Backpack (" << inventory.size() << " items):" << std::endl;
    
    // A more modern way to iterate: The "Range-based for loop"
    // "For every item in the inventory..."
    for (std::string item : inventory) {
        std::cout << "- " << item << std::endl;
    }

    return 0;
}
