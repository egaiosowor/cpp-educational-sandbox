#include "raylib.h"
struct Paddle { float x, y, width, height, speed; };
struct Ball { float x, y, radius, speedX, speedY; };
int main() {
    InitWindow(800, 600, "Pong!");
    Paddle p1 = { 30, 250, 20, 100, 400 };
    Paddle p2 = { 750, 250, 20, 100, 400 };
    Ball b = { 400, 300, 10, 300, 300 };
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        if (IsKeyDown(KEY_W)) p1.y -= p1.speed * dt;
        if (IsKeyDown(KEY_S)) p1.y += p1.speed * dt;
        if (IsKeyDown(KEY_UP)) p2.y -= p2.speed * dt;
        if (IsKeyDown(KEY_DOWN)) p2.y += p2.speed * dt;
        b.x += b.speedX * dt; b.y += b.speedY * dt;
        BeginDrawing();
        ClearBackground(BLACK);
        DrawRectangle(p1.x, p1.y, p1.width, p1.height, WHITE);
        DrawRectangle(p2.x, p2.y, p2.width, p2.height, WHITE);
        DrawCircle(b.x, b.y, b.radius, WHITE);
        EndDrawing();
    }
    return 0;
}
