# Monster Clicker (GUI)

![Monster Clicker Screenshot](./app.png)

Welcome to your very first graphical game! We are moving out of the console and drawing real shapes on the screen using a library called **Raylib**.

## About the Game

Monster Clicker is an incremental "clicker" game where your goal is to amass as much Gold as possible.

- **Click the Monster:** Every time you click the green monster, you earn Gold.
- **Buy Upgrades:** Once you earn 50 Gold, you can visit the shop to buy an Auto-Clicker.
- **Passive Income:** Every Auto-Clicker you own automatically mines Gold for you every single second, even while you rest!

## Learning Objective

The goal of this project is to transition from text-based terminal applications to real-time graphical applications. It demonstrates how to set up a blank window, draw shapes to the screen, and create an interactive user interface using C++.

## What this game teaches:

- **The Game Loop:** You'll see a `while (!WindowShouldClose())` loop. This runs 60 times every second! Inside it, we check for inputs, update our math, and then draw the graphics.
- **Coordinate Systems:** Unlike the console where text just flows down, graphics are drawn at specific X and Y coordinates (like a graph). `x: 0, y: 0` is the top-left corner of the window.
- **Collision Detection:** We use basic math (`CheckCollisionPointCircle` and `CheckCollisionPointRec`) to figure out if your mouse cursor is clicking inside the monster or the shop buttons.

## Prerequisites

This project uses **CMake** to automatically fetch the Raylib graphics library from the internet. You do not need to install Raylib manually! However, you must have CMake installed on your system.

**Mac:**

```bash
brew install cmake
```

**Windows:**
Download and install CMake from the official website: [cmake.org/download](https://cmake.org/download/)
_(Make sure to check the box "Add CMake to the system PATH for all users" during installation)._

**Linux (Ubuntu/Debian):**

```bash
sudo apt update
sudo apt install cmake build-essential libgl1-mesa-dev libx11-dev libxrandr-dev libxi-dev libxcursor-dev libxinerama-dev libxxf86vm-dev
```

_(Note: Linux requires a few extra windowing packages for Raylib to compile successfully)._

## How to Build and Run

We use a "build" folder to keep all the messy compiled files separate from our clean C++ code.

1. Open your terminal and navigate to this folder:
   ```bash
   cd games/01_clicker
   ```
2. Create the build directory and enter it:
   ```bash
   mkdir build
   cd build
   ```
3. Tell CMake to configure the project (it will download Raylib for you):
   ```bash
   cmake ..
   ```
4. Compile the game:
   ```bash
   make
   ```
5. Play!
   ```bash
   ./MonsterClicker
   ```

_(Tip: In the future, you only need to run step 4 and 5 after changing the code in `main.cpp`!)_
