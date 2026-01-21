# C++ Beginner-to-Pro RPG Curriculum

Welcome to the C++ Beginner-to-Pro RPG Curriculum! This 9-part series is designed to take you from absolute zero programming experience to understanding advanced C++ concepts like Pointers, Vectors, Inheritance, and Polymorphism. 

To keep learning fun, every lesson is themed around building components for a text-based **Console RPG Game**.

## Course Structure

The curriculum is split into 9 standalone lessons. You can compile and run each one independently.

1. **`01_computing_and_datatypes.cpp`**
   - *Topic:* What is a C++ program? Primitive Data Types (`int`, `float`, `char`, `bool`).
   - *RPG Theme:* Defining the raw stats of our game world.
2. **`02_variables_and_io.cpp`**
   - *Topic:* Variables, `std::string`, `std::cout`, `std::cin`.
   - *RPG Theme:* Creating your character and interacting with the Tavern Keeper.
3. **`03_control_flow.cpp`**
   - *Topic:* `if`, `else if`, `else`, and `while` loops.
   - *RPG Theme:* A basic text adventure loop and fighting a Goblin.
4. **`04_functions.cpp`**
   - *Topic:* Function declarations, definitions, return types, and arguments.
   - *RPG Theme:* Reusable skills like `calculateDamage()` and `healPlayer()`.
5. **`05_structs_and_classes.cpp`**
   - *Topic:* Object-Oriented Programming (OOP) basics, access modifiers, constructors.
   - *RPG Theme:* Grouping health and stats into `Player` and `Weapon` objects.
6. **`06_pointers_and_references.cpp`**
   - *Topic:* Memory addresses, Pointers (`*`), References (`&`), and dynamic allocation (`new`/`delete`).
   - *RPG Theme:* Spawning dynamic "Loot" drops on the ground and passing players by reference to heal them.
7. **`07_vectors.cpp`**
   - *Topic:* `std::vector`, dynamic arrays, and range-based for loops.
   - *RPG Theme:* An infinite capacity inventory system (the Adventurer's Backpack).
8. **`08_inheritance.cpp`**
   - *Topic:* Base classes, Derived classes, and `protected` members.
   - *RPG Theme:* Building a Bestiary with a base `Enemy` class and derived `Goblin` and `Dragon` classes.
9. **`09_polymorphism.cpp`**
   - *Topic:* Virtual functions, overriding, and arrays of pointers to base classes.
   - *RPG Theme:* Epic Encounters! Storing an array of mixed enemies and using a single loop to trigger their unique attacks.

## How to Run

You can compile any of the lessons using a C++ compiler like `clang++` or `g++`.

```bash
# Example: Running Lesson 1
clang++ -std=c++17 01_computing_and_datatypes.cpp -o lesson1
./lesson1
```

Happy coding, adventurer!
