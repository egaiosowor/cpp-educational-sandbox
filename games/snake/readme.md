# Snake

Welcome to the third graphical game in the C++ curriculum! In this project, we are building a grid-based clone of the iconic Snake game.

![Snake Gameplay](app.png)

## About the Game

Snake is a game about memory management and grid math!
- **Objective:** Eat the red food squares to grow your snake and increase your score.
- **Controls:** Use `W, A, S, D` or the `Arrow Keys` to change direction. You cannot instantly reverse into yourself!
- **Game Over:** If your snake's head touches the edge of the screen or collides with any part of its own body, it's game over!

## Learning Objective

This project builds heavily upon the **Vectors** lesson. The primary challenge of Snake is that the player's character is constantly changing in size, meaning we cannot use fixed-size arrays or simple variables to track it.

## What this game teaches:

- **Dynamic Arrays (`std::vector`):** Using a C++ vector to store an ever-growing list of `Vector2` coordinates. Each time the snake eats food, we use `.push_back()` to add a new segment to the end of the vector.
- **Grid-Based Mathematics:** Instead of moving smoothly on every frame, the snake "snaps" to a grid. We divide the 800x600 screen into 20x20 pixel cells, and multiply random values by 20 to ensure food always spawns perfectly aligned to the grid.
- **Artificial Timers:** Raylib runs very fast (60+ frames per second). We create a custom `updateTimer` variable to only allow the snake to move forward once every 0.1 seconds, giving it that classic chunky arcade feel.
- **Body Shifting Logic:** To move the snake, we loop through the vector backwards, copying the position of the segment in front of it, and then finally move the head. 

## How to Build and Run

1. Open your terminal and navigate to the snake folder:
   ```bash
   cd games/snake
   ```
2. Create the build directory and enter it:
   ```bash
   mkdir build
   cd build
   ```
3. Tell CMake to configure the project (it will download Raylib for you):
   ```bash
   cmake -DCMAKE_POLICY_VERSION_MINIMUM=3.5 ..
   ```
4. Compile the game:
   ```bash
   make
   ```
5. Play!
   ```bash
   ./Snake
   ```
