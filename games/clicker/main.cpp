#include "raylib.h"
int main() {
    InitWindow(800, 600, "Monster Clicker");
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Monster Clicker", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
