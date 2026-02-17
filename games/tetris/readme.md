# Tetris

Welcome to the fourth graphical game in our curriculum! In this project, we are building a fully playable clone of the legendary Tetris.

![Tetris Gameplay](app.png)

## About the Game

- **Objective:** Arrange falling tetrominoes to complete horizontal lines. Completed lines are cleared from the board, earning you points!
- **Controls:**
  - **Move:** `A` / `D` or `Left` / `Right`
  - **Soft Drop:** `S` or `Down`
  - **Hard Drop:** `Space`
  - **Rotate:** `W` or `Up`
  - **Pause:** `P` | **Quit:** `Q`
- **Leveling:** Every 10 lines cleared increases the level and speeds up the drop rate!

## What this game teaches:

- **2D Arrays as Game Boards:** The entire Tetris board is a `int[10][20]` array. Each cell stores either `0` (empty) or a piece type index. This is the most direct and powerful demonstration of using 2D arrays to represent visual state.
- **Rotation Mathematics:** Rotating a tetromino 90° clockwise is achieved by transposing the 4x4 grid: `rotated[x][3-y] = original[y][x]`. This is a real-world application of matrix transformation.
- **Collision Detection on a Grid:** Before every move (left, right, down, rotate), we must check whether the new position would overlap with the walls, floor, or any locked piece on the board.
- **Line Clearing Algorithm:** After locking a piece, we scan every row from bottom to top. If a row is completely full, we shift every row above it down by one — a classic array manipulation exercise.
- **Ghost Piece:** A transparent preview of where your piece will land, calculated by simulating a hard drop without committing it.
