#include "raylib.h"
#include <string>

// --- Game Structures ---
enum GameState { MENU, PLAYING, PAUSED, GAME_OVER };

struct Paddle {
    float x, y;
    float width, height;
    float speed;
};

struct Ball {
    float x, y;
    float radius;
    float speedX, speedY;
};

// --- Helper Functions ---
void ResetGame(Ball& ball, Paddle& p1, Paddle& p2, int& s1, int& s2, const int screenW, const int screenH) {
    s1 = 0; s2 = 0;
    
    p1.y = screenH / 2.0f - p1.height / 2.0f;
    p2.y = screenH / 2.0f - p2.height / 2.0f;

    ball.x = screenW / 2.0f;
    ball.y = screenH / 2.0f;
    ball.speedX = 300.0f;
    ball.speedY = 300.0f;
}

int main() {
    // --- Initialization ---
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Pong!");
    SetExitKey(0); // Prevent ESCAPE from instantly closing the window
    InitAudioDevice(); // Initialize audio engine
    SetTargetFPS(60);

    // Load Sounds
    Sound hitSound = LoadSound("paddle_hit.wav");
    Sound scoreSound = LoadSound("wall_hit.wav");

    // Game State
    GameState state = MENU;
    bool isSinglePlayer = true;
    bool isHardMode = false;
    int winner = 0; // 1 or 2

    // Objects
    Paddle player1 = { 30, screenHeight/2.0f - 50, 20, 100, 400.0f };
    Paddle player2 = { screenWidth - 50, screenHeight/2.0f - 50, 20, 100, 400.0f };
    Ball ball = { screenWidth/2.0f, screenHeight/2.0f, 10.0f, 300.0f, 300.0f };

    int score1 = 0;
    int score2 = 0;

    // --- Main Game Loop ---
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // ==========================================
        // MENU LOGIC
        // ==========================================
        if (state == MENU) {
            // Toggle Mode
            if (IsKeyPressed(KEY_ONE)) isSinglePlayer = true;
            if (IsKeyPressed(KEY_TWO)) isSinglePlayer = false;
            
            // Toggle Difficulty
            if (IsKeyPressed(KEY_E)) isHardMode = false;
            if (IsKeyPressed(KEY_H)) isHardMode = true;

            // Start Game
            if (IsKeyPressed(KEY_SPACE)) {
                ResetGame(ball, player1, player2, score1, score2, screenWidth, screenHeight);
                state = PLAYING;
            }

            BeginDrawing();
                ClearBackground(BLACK);
                DrawText("PONG", screenWidth/2 - MeasureText("PONG", 60)/2, 100, 60, RAYWHITE);
                
                DrawText("Select Mode:", 100, 250, 30, LIGHTGRAY);
                DrawText(isSinglePlayer ? "> [1] Single Player" : "  [1] Single Player", 120, 290, 20, isSinglePlayer ? YELLOW : DARKGRAY);
                DrawText(!isSinglePlayer ? "> [2] Multiplayer" : "  [2] Multiplayer", 120, 320, 20, !isSinglePlayer ? YELLOW : DARKGRAY);

                if (isSinglePlayer) {
                    DrawText("Select Difficulty:", 450, 250, 30, LIGHTGRAY);
                    DrawText(!isHardMode ? "> [E] Easy" : "  [E] Easy", 470, 290, 20, !isHardMode ? GREEN : DARKGRAY);
                    DrawText(isHardMode ? "> [H] Hard" : "  [H] Hard", 470, 320, 20, isHardMode ? RED : DARKGRAY);
                }

                DrawText("Controls:", 100, 400, 20, LIGHTGRAY);
                DrawText("Player 1: W / S", 100, 430, 20, DARKGRAY);
                DrawText(isSinglePlayer ? "Player 2: AI" : "Player 2: UP / DOWN", 100, 460, 20, DARKGRAY);

                DrawText("Game Controls:", 450, 400, 20, LIGHTGRAY);
                DrawText("P: Pause / Play", 450, 430, 20, DARKGRAY);
                DrawText("R: Restart", 450, 460, 20, DARKGRAY);
                DrawText("Q: Quit to Menu", 450, 490, 20, DARKGRAY);

                DrawText("Press SPACE to Start", screenWidth/2 - MeasureText("Press SPACE to Start", 30)/2, 530, 30, RAYWHITE);
            EndDrawing();
        }
        
        // ==========================================
        // PLAYING LOGIC
        // ==========================================
        else if (state == PLAYING) {
            
            // Menu / Restart / Pause Logic
            if (IsKeyPressed(KEY_Q)) state = MENU;
            if (IsKeyPressed(KEY_R)) {
                ResetGame(ball, player1, player2, score1, score2, screenWidth, screenHeight);
            }
            if (IsKeyPressed(KEY_P)) {
                state = PAUSED;
            }

            if (state == PLAYING) {
                // Player 1 Input
                if (IsKeyDown(KEY_W)) player1.y -= player1.speed * dt;
                if (IsKeyDown(KEY_S)) player1.y += player1.speed * dt;

                // Player 2 Input / AI
                if (!isSinglePlayer) {
                    if (IsKeyDown(KEY_UP)) player2.y -= player2.speed * dt;
                    if (IsKeyDown(KEY_DOWN)) player2.y += player2.speed * dt;
                } else {
                    // Basic AI Tracking
                    float aiSpeed = isHardMode ? 450.0f : 200.0f;
                    float paddleCenterY = player2.y + (player2.height / 2.0f);
                    if (paddleCenterY < ball.y - 10) player2.y += aiSpeed * dt;
                    else if (paddleCenterY > ball.y + 10) player2.y -= aiSpeed * dt;
                }

                // Screen bounds
                if (player1.y < 0) player1.y = 0;
                if (player1.y > screenHeight - player1.height) player1.y = screenHeight - player1.height;
                if (player2.y < 0) player2.y = 0;
                if (player2.y > screenHeight - player2.height) player2.y = screenHeight - player2.height;

                // Ball movement
                ball.x += ball.speedX * dt;
                ball.y += ball.speedY * dt;

                // Wall collisions
                if (ball.y - ball.radius <= 0) {
                    ball.y = ball.radius;
                    ball.speedY *= -1;
                    PlaySound(hitSound);
                } else if (ball.y + ball.radius >= screenHeight) {
                    ball.y = screenHeight - ball.radius;
                    ball.speedY *= -1;
                    PlaySound(hitSound);
                }

                // Paddle collisions with Dynamic Bounce
                Rectangle rect1 = { player1.x, player1.y, player1.width, player1.height };
                if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, rect1) && ball.speedX < 0) {
                    ball.speedX *= -1.05f; // Reverse and speed up
                    
                    // Dynamic vertical bounce based on hit location
                    float hitFactor = (ball.y - (player1.y + player1.height/2.0f)) / (player1.height/2.0f);
                    ball.speedY = hitFactor * 400.0f; // Sharper angle if near edge
                    
                    PlaySound(hitSound);
                }

                Rectangle rect2 = { player2.x, player2.y, player2.width, player2.height };
                if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, rect2) && ball.speedX > 0) {
                    ball.speedX *= -1.05f; 
                    
                    float hitFactor = (ball.y - (player2.y + player2.height/2.0f)) / (player2.height/2.0f);
                    ball.speedY = hitFactor * 400.0f; 
                    
                    PlaySound(hitSound);
                }

                // Scoring
                if (ball.x < 0) {
                    score2++;
                    PlaySound(scoreSound);
                    ball.x = screenWidth / 2.0f; ball.y = screenHeight / 2.0f;
                    ball.speedX = 300.0f; ball.speedY = 300.0f;
                } else if (ball.x > screenWidth) {
                    score1++;
                    PlaySound(scoreSound);
                    ball.x = screenWidth / 2.0f; ball.y = screenHeight / 2.0f;
                    ball.speedX = -300.0f; ball.speedY = 300.0f;
                }

                // Win Condition
                if (score1 >= 10) { winner = 1; state = GAME_OVER; }
                if (score2 >= 10) { winner = 2; state = GAME_OVER; }
            }

            // Render
            BeginDrawing();
                ClearBackground(BLACK);
                
                // Bottom Controls
                const char* controlsText = "Q [Quit]  R [Restart]  P [Pause/Play]";
                DrawText(controlsText, screenWidth/2 - MeasureText(controlsText, 20)/2, screenHeight - 30, 20, Fade(LIGHTGRAY, 0.6f));

                DrawText(TextFormat("%d", score1), screenWidth / 4, 20, 80, DARKGRAY);
                DrawText(TextFormat("%d", score2), 3 * screenWidth / 4, 20, 80, DARKGRAY);
                for (int i = 0; i < screenHeight; i += 40) DrawRectangle(screenWidth / 2 - 2, i, 4, 20, LIGHTGRAY);
                
                DrawRectangle(player1.x, player1.y, player1.width, player1.height, WHITE);
                DrawRectangle(player2.x, player2.y, player2.width, player2.height, WHITE);
                DrawCircle(ball.x, ball.y, ball.radius, WHITE);
            EndDrawing();
        }

        // ==========================================
        // PAUSED LOGIC
        // ==========================================
        else if (state == PAUSED) {
            if (IsKeyPressed(KEY_P)) state = PLAYING;
            if (IsKeyPressed(KEY_Q)) state = MENU;
            if (IsKeyPressed(KEY_R)) {
                ResetGame(ball, player1, player2, score1, score2, screenWidth, screenHeight);
                state = PLAYING;
            }

            BeginDrawing();
                ClearBackground(BLACK);
                
                // Draw statically
                DrawText(TextFormat("%d", score1), screenWidth / 4, 20, 80, DARKGRAY);
                DrawText(TextFormat("%d", score2), 3 * screenWidth / 4, 20, 80, DARKGRAY);
                for (int i = 0; i < screenHeight; i += 40) DrawRectangle(screenWidth / 2 - 2, i, 4, 20, LIGHTGRAY);
                
                DrawRectangle(player1.x, player1.y, player1.width, player1.height, WHITE);
                DrawRectangle(player2.x, player2.y, player2.width, player2.height, WHITE);
                DrawCircle(ball.x, ball.y, ball.radius, WHITE);

                // Overlay
                DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.8f));
                DrawText("PAUSED", screenWidth/2 - MeasureText("PAUSED", 60)/2, 200, 60, RAYWHITE);
                DrawText("Press P to Resume", screenWidth/2 - MeasureText("Press P to Resume", 30)/2, 300, 30, LIGHTGRAY);
                DrawText("Press R to Restart", screenWidth/2 - MeasureText("Press R to Restart", 30)/2, 350, 30, LIGHTGRAY);
                DrawText("Press Q to Quit to Menu", screenWidth/2 - MeasureText("Press Q to Quit to Menu", 30)/2, 400, 30, LIGHTGRAY);
            EndDrawing();
        }
        
        // ==========================================
        // GAME OVER LOGIC
        // ==========================================
        else if (state == GAME_OVER) {
            if (IsKeyPressed(KEY_Q)) state = MENU;
            if (IsKeyPressed(KEY_R)) {
                ResetGame(ball, player1, player2, score1, score2, screenWidth, screenHeight);
                state = PLAYING;
            }

            BeginDrawing();
                ClearBackground(BLACK);
                const char* winText = (winner == 1) ? "PLAYER 1 WINS!" : "PLAYER 2 WINS!";
                DrawText(winText, screenWidth/2 - MeasureText(winText, 60)/2, 200, 60, GOLD);
                DrawText("Press R to Restart", screenWidth/2 - MeasureText("Press R to Restart", 30)/2, 400, 30, RAYWHITE);
                DrawText("Press Q to Quit to Menu", screenWidth/2 - MeasureText("Press Q to Quit to Menu", 30)/2, 450, 30, RAYWHITE);
            EndDrawing();
        }
    }

    // --- Cleanup ---
    UnloadSound(hitSound);
    UnloadSound(scoreSound);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
