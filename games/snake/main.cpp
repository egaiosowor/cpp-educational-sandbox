#include "raylib.h"
#include <vector>
#include <string>

// --- Game Settings ---
const int screenWidth = 800;
const int screenHeight = 600;
const int cellSize = 20;

// Game states
enum GameState {
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER
};

// --- Game Logic Functions ---
void ResetGame(std::vector<Vector2>& snake, Vector2& food, Vector2& speed, int& score) {
    snake.clear();
    // Start with 3 segments
    snake.push_back({ (float)screenWidth / 2, (float)screenHeight / 2 });
    snake.push_back({ (float)screenWidth / 2 - cellSize, (float)screenHeight / 2 });
    snake.push_back({ (float)screenWidth / 2 - 2 * cellSize, (float)screenHeight / 2 });

    speed = { (float)cellSize, 0 }; // Moving right
    score = 0;

    food.x = GetRandomValue(0, (screenWidth / cellSize) - 1) * cellSize;
    food.y = GetRandomValue(0, (screenHeight / cellSize) - 1) * cellSize;
}

int main() {
    InitWindow(screenWidth, screenHeight, "Snake!");
    InitAudioDevice();

    Sound eatSound = LoadSound("eat.wav");
    Sound crashSound = LoadSound("crash.wav");

    GameState currentState = MENU;

    std::vector<Vector2> snake;
    Vector2 food = {0, 0};
    Vector2 speed = {0, 0};
    int score = 0;
    int highScore = 0;

    // Timer for grid-snapping movement
    float updateTimer = 0.0f;
    const float updateInterval = 0.1f; // Move every 0.1 seconds

    ResetGame(snake, food, speed, score);

    // Prevent ESC from exiting the game immediately
    SetExitKey(0);

    // --- Main Game Loop ---
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // --- Logic Update ---
        if (currentState == MENU) {
            if (IsKeyPressed(KEY_SPACE)) {
                ResetGame(snake, food, speed, score);
                currentState = PLAYING;
            }
        }
        else if (currentState == GAME_OVER) {
            if (IsKeyPressed(KEY_R)) {
                ResetGame(snake, food, speed, score);
                currentState = PLAYING;
            }
            if (IsKeyPressed(KEY_Q)) {
                currentState = MENU;
            }
        }
        else if (currentState == PAUSED) {
            if (IsKeyPressed(KEY_P)) {
                currentState = PLAYING;
            }
            if (IsKeyPressed(KEY_Q)) {
                currentState = MENU;
            }
        }
        else if (currentState == PLAYING) {
            // Input handling
            // We check the physical direction of the head vs the first body segment
            // to prevent the player from instantly reversing into themselves by pressing two keys rapidly
            Vector2 currentDir = { snake[0].x - snake[1].x, snake[0].y - snake[1].y };
            
            if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) && currentDir.y == 0) speed = {0, (float)-cellSize};
            if ((IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) && currentDir.y == 0) speed = {0, (float)cellSize};
            if ((IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) && currentDir.x == 0) speed = {(float)-cellSize, 0};
            if ((IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) && currentDir.x == 0) speed = {(float)cellSize, 0};

            // Global Game Controls
            if (IsKeyPressed(KEY_Q)) {
                currentState = MENU;
                continue;
            }
            if (IsKeyPressed(KEY_P)) {
                currentState = PAUSED;
                continue;
            }

            // Movement Timer
            updateTimer += dt;
            if (updateTimer >= updateInterval) {
                updateTimer = 0.0f;

                // Move body: shift everything one position forward
                for (int i = snake.size() - 1; i > 0; i--) {
                    snake[i] = snake[i - 1];
                }

                // Move head
                snake[0].x += speed.x;
                snake[0].y += speed.y;

                // Check Wall Collision
                if (snake[0].x < 0 || snake[0].x >= screenWidth || 
                    snake[0].y < 0 || snake[0].y >= screenHeight) {
                    PlaySound(crashSound);
                    currentState = GAME_OVER;
                }

                // Check Self Collision
                for (size_t i = 1; i < snake.size(); i++) {
                    if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
                        PlaySound(crashSound);
                        currentState = GAME_OVER;
                    }
                }

                // Check Food Collision
                if (snake[0].x == food.x && snake[0].y == food.y) {
                    PlaySound(eatSound);
                    score += 10;
                    if (score > highScore) highScore = score;
                    // Add a new segment at the end (position doesn't matter, it will move into place next frame)
                    snake.push_back(snake.back());
                    
                    // Spawn new food, ensuring it aligns with the grid
                    food.x = GetRandomValue(0, (screenWidth / cellSize) - 1) * cellSize;
                    food.y = GetRandomValue(0, (screenHeight / cellSize) - 1) * cellSize;
                }
            }
        }

        // --- Render ---
        BeginDrawing();
        ClearBackground(BLACK);

        if (currentState == MENU) {
            DrawText("SNAKE", screenWidth/2 - MeasureText("SNAKE", 60)/2, 100, 60, GREEN);
            
            DrawText(TextFormat("High Score: %d", highScore), screenWidth/2 - MeasureText(TextFormat("High Score: %d", highScore), 30)/2, 180, 30, YELLOW);

            DrawText("Controls:", 300, 300, 20, LIGHTGRAY);
            DrawText("Move: W A S D  or  Arrows", 300, 330, 20, DARKGRAY);
            DrawText("Pause: P", 300, 360, 20, DARKGRAY);
            DrawText("Quit to Menu: Q", 300, 390, 20, DARKGRAY);

            DrawText("Press SPACE to Start", screenWidth/2 - MeasureText("Press SPACE to Start", 30)/2, 480, 30, RAYWHITE);
        }
        else if (currentState == PLAYING || currentState == PAUSED) {
            // Draw Food
            DrawRectangle(food.x, food.y, cellSize, cellSize, RED);

            // Draw Snake
            for (size_t i = 0; i < snake.size(); i++) {
                Color segmentColor = (i == 0) ? GREEN : LIME;
                DrawRectangle(snake[i].x, snake[i].y, cellSize, cellSize, segmentColor);
            }

            // Draw Score
            DrawText(TextFormat("Score: %d   High Score: %d", score, highScore), 10, 10, 20, LIGHTGRAY);

            if (currentState == PAUSED) {
                DrawText("PAUSED", screenWidth/2 - MeasureText("PAUSED", 60)/2, 200, 60, LIGHTGRAY);
                DrawText("Press [P] to Resume", screenWidth/2 - MeasureText("Press [P] to Resume", 20)/2, 280, 20, DARKGRAY);
                DrawText("Press [Q] to Quit to Menu", screenWidth/2 - MeasureText("Press [Q] to Quit to Menu", 20)/2, 310, 20, DARKGRAY);
            }
        }
        else if (currentState == GAME_OVER) {
            DrawText("GAME OVER", screenWidth/2 - MeasureText("GAME OVER", 60)/2, 180, 60, RED);
            DrawText(TextFormat("Final Score: %d", score), screenWidth/2 - MeasureText(TextFormat("Final Score: %d", score), 30)/2, 260, 30, LIGHTGRAY);
            DrawText(TextFormat("High Score: %d", highScore), screenWidth/2 - MeasureText(TextFormat("High Score: %d", highScore), 30)/2, 300, 30, YELLOW);
            
            DrawText("Press [R] to Restart", screenWidth/2 - MeasureText("Press [R] to Restart", 20)/2, 400, 20, DARKGRAY);
            DrawText("Press [Q] to Quit to Menu", screenWidth/2 - MeasureText("Press [Q] to Quit to Menu", 20)/2, 430, 20, DARKGRAY);
        }

        EndDrawing();
    }

    UnloadSound(eatSound);
    UnloadSound(crashSound);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
