# Game of Life (Grid Visualizer)

Welcome to the advanced algorithm visualizer project! Instead of building a game with sprites, we are going to use the power of C++ to visualize abstract cellular math on a massive grid. 

![Game of Life Simulation](app.png)

## About the Simulation
This project implements **Conway's Game of Life**, a cellular automaton where a grid of cells lives, dies, and reproduces based on a set of mathematical rules. 

- **Controls:** Use the Mouse to draw living cells (Left Click) or erase them (Right Click).
- **Play/Pause:** Press `SPACE` to toggle the simulation. When running, the cells will automatically update.
- **Clear:** Press `C` while paused to clear the entire grid.

## The CS Concepts

- **2D Arrays (`std::vector<std::vector<int>>`):** The primary data structure is a massive 2D array. This teaches how to map visual spatial coordinates (X, Y) directly to array indices `[row][col]`.
- **Neighborhood Checking Loops:** To determine if a cell lives or dies, we use nested `for` loops to iterate through the 8 adjacent cells surrounding it.
- **Discrete Logic Processing:** The simulation uses a "double buffer" approach. We calculate the state of the *entire* next grid before swapping it with the current grid, ensuring that updates happen simultaneously rather than bleeding into each other sequentially.
