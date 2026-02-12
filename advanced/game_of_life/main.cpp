#include "raylib.h"
#include <vector>

const int screenWidth = 800;
const int screenHeight = 600;
const int cellSize = 20;
const int cols = screenWidth / cellSize;
const int rows = screenHeight / cellSize;

// Count alive neighbors around a specific cell
int CountNeighbors(const std::vector<std::vector<int>>& grid, int x, int y) {
    int sum = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            // Wrap around edges (Toroidal array)
            int col = (x + i + cols) % cols;
            int row = (y + j + rows) % rows;
            sum += grid[col][row];
        }
    }
    // Subtract the cell itself so we don't count it as a neighbor
    sum -= grid[x][y];
    return sum;
}

int main() {
    InitWindow(screenWidth, screenHeight, "Conway's Game of Life");

    // Initialize 2D Grids
    std::vector<std::vector<int>> grid(cols, std::vector<int>(rows, 0));
    std::vector<std::vector<int>> nextGrid = grid;

    bool isPlaying = false;
    float updateTimer = 0.0f;
    const float updateInterval = 0.1f;

    while (!WindowShouldClose()) {
        // --- Input & Logic ---
        if (IsKeyPressed(KEY_SPACE)) {
            isPlaying = !isPlaying; // Toggle play/pause
        }
        
        if (IsKeyPressed(KEY_C)) {
            // Clear the grid
            for (int i = 0; i < cols; i++) {
                for (int j = 0; j < rows; j++) {
                    grid[i][j] = 0;
                }
            }
            isPlaying = false;
        }

        if (!isPlaying) {
            // Drawing Mode: Left click to draw, Right click to erase
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                Vector2 mousePos = GetMousePosition();
                int col = mousePos.x / cellSize;
                int row = mousePos.y / cellSize;
                if (col >= 0 && col < cols && row >= 0 && row < rows) {
                    grid[col][row] = 1;
                }
            }
            if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
                Vector2 mousePos = GetMousePosition();
                int col = mousePos.x / cellSize;
                int row = mousePos.y / cellSize;
                if (col >= 0 && col < cols && row >= 0 && row < rows) {
                    grid[col][row] = 0;
                }
            }
        } else {
            // Simulation Mode: Run Conway's Logic
            updateTimer += GetFrameTime();
            if (updateTimer >= updateInterval) {
                updateTimer = 0.0f;

                // Loop through every cell
                for (int i = 0; i < cols; i++) {
                    for (int j = 0; j < rows; j++) {
                        int state = grid[i][j];
                        int neighbors = CountNeighbors(grid, i, j);

                        // Conway's Rules of Life
                        if (state == 0 && neighbors == 3) {
                            nextGrid[i][j] = 1; // Reproduction
                        } else if (state == 1 && (neighbors < 2 || neighbors > 3)) {
                            nextGrid[i][j] = 0; // Underpopulation or Overpopulation
                        } else {
                            nextGrid[i][j] = state; // Stays the same
                        }
                    }
                }

                // Swap grids for the next generation
                grid = nextGrid;
            }
        }

        // --- Render ---
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw Cells
        for (int i = 0; i < cols; i++) {
            for (int j = 0; j < rows; j++) {
                if (grid[i][j] == 1) {
                    DrawRectangle(i * cellSize, j * cellSize, cellSize - 1, cellSize - 1, DARKGRAY);
                } else {
                    // Draw grid lines
                    DrawRectangle(i * cellSize, j * cellSize, cellSize - 1, cellSize - 1, LIGHTGRAY);
                }
            }
        }

        // UI Text
        DrawRectangle(0, screenHeight - 40, screenWidth, 40, Fade(BLACK, 0.8f));
        if (isPlaying) {
            DrawText("PLAYING - Press SPACE to Pause", 10, screenHeight - 30, 20, GREEN);
        } else {
            DrawText("PAUSED - Draw with Mouse - Press SPACE to Play - Press C to Clear", 10, screenHeight - 30, 20, YELLOW);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
