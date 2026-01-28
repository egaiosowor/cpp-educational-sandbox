#include "raylib.h"
#include <string>

int main() {
    // --- Initialization ---
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Monster Clicker!");
    SetTargetFPS(60); // Run at 60 frames per second

    // Game Variables
    int gold = 0;
    int goldPerClick = 1;
    
    int autoClickers = 0;
    int autoClickerCost = 50;

    // Monster settings
    Vector2 monsterPosition = { screenWidth / 2.0f, screenHeight / 2.0f };
    float monsterRadius = 100.0f;
    Color monsterColor = GREEN;

    // Shop settings
    Rectangle shopButton = { screenWidth - 250.0f, 50.0f, 220.0f, 60.0f };

    // Timer for auto-clicker
    float timeSinceLastTick = 0.0f;

    // --- Main Game Loop ---
    while (!WindowShouldClose()) {
        // --- 1. Update Logic ---
        
        // Handle Auto-Clickers (give gold every second)
        timeSinceLastTick += GetFrameTime();
        if (timeSinceLastTick >= 1.0f) {
            gold += autoClickers;
            timeSinceLastTick -= 1.0f;
        }

        // Handle Mouse Clicks
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mousePos = GetMousePosition();

            // Check if we clicked the monster (using distance from center)
            if (CheckCollisionPointCircle(mousePos, monsterPosition, monsterRadius)) {
                gold += goldPerClick;
                monsterColor = LIME; // Visual feedback when clicked
            }

            // Check if we clicked the shop button
            if (CheckCollisionPointRec(mousePos, shopButton)) {
                if (gold >= autoClickerCost) {
                    gold -= autoClickerCost;
                    autoClickers++;
                    autoClickerCost += 20; // Price goes up!
                }
            }
        }

        // Reset monster color when mouse is released
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            monsterColor = GREEN;
        }

        // --- 2. Draw Graphics ---
        BeginDrawing();
            ClearBackground(RAYWHITE);

            // Draw Gold Score
            DrawText(TextFormat("Gold: %d", gold), 20, 20, 40, GOLD);
            DrawText(TextFormat("Auto-Clickers: %d", autoClickers), 20, 70, 20, DARKGRAY);

            // Draw Monster (Circle)
            DrawCircleV(monsterPosition, monsterRadius, monsterColor);
            
            // Draw face on monster (just for fun)
            DrawCircle(monsterPosition.x - 30, monsterPosition.y - 20, 15, WHITE); // Left Eye
            DrawCircle(monsterPosition.x + 30, monsterPosition.y - 20, 15, WHITE); // Right Eye
            DrawCircle(monsterPosition.x - 30, monsterPosition.y - 20, 5, BLACK);  // Left Pupil
            DrawCircle(monsterPosition.x + 30, monsterPosition.y - 20, 5, BLACK);  // Right Pupil
            DrawRectangle(monsterPosition.x - 40, monsterPosition.y + 20, 80, 10, BLACK); // Mouth

            // Draw Shop Button
            Color buttonColor = (gold >= autoClickerCost) ? SKYBLUE : LIGHTGRAY;
            DrawRectangleRec(shopButton, buttonColor);
            DrawText("Buy Auto-Clicker", shopButton.x + 10, shopButton.y + 10, 20, BLACK);
            DrawText(TextFormat("Cost: %d Gold", autoClickerCost), shopButton.x + 10, shopButton.y + 35, 15, DARKGRAY);

        EndDrawing();
    }

    // --- Cleanup ---
    CloseWindow();

    return 0;
}
