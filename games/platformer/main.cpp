#include "raylib.h"
#include <vector>
#include <string>
#include <cmath>

// --- Constants ---
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int TILE_SIZE = 40;

const float GRAVITY = 1500.0f;
const float JUMP_FORCE = -650.0f;
const float MOVE_SPEED = 300.0f;

enum GameState {
    MENU,
    PLAYING,
    GAME_OVER,
    LEVEL_COMPLETE
};

// --- Game Objects ---
struct Player {
    Vector2 position;
    Vector2 velocity;
    int score;
    bool isGrounded;
};

struct Enemy {
    Vector2 position;
    Vector2 velocity;
    bool isAlive;
};

struct Cloud {
    Vector2 position;
    float speed;
    float scale;
};

// --- The Level Map ---
// '#' = Solid Block
// 'C' = Coin
// 'P' = Player Start
// 'E' = Enemy (Goomba)
// 'F' = Finish Flag
std::vector<std::string> levelMap = {
    "################################################################################",
    "#                                                                              #",
    "#                                                                              #",
    "#                                                                              #",
    "#                                                                              #",
    "#                                                                              #",
    "#                                                                              #",
    "#                                         C C C                                #",
    "#                                        #######                               #",
    "#         C C C                 C                      E                       #",
    "#        #######               ####                 #######                    #",
    "#                                                                              #",
    "# P               E                                   C  C                   F #",
    "######################    ######################################################",
    "######################    ######################################################"
};

const int MAP_WIDTH = levelMap[0].size();
const int MAP_HEIGHT = levelMap.size();

std::vector<Enemy> enemies;
std::vector<Cloud> clouds;

const int CLOUD_WIDTH = 21;
const int CLOUD_HEIGHT = 10;
const char* cloudTemplate[CLOUD_HEIGHT] = {
    "        WWWW         ",
    "      WWWWWWWW       ",
    "    WWWWWWWWWWWW     ",
    "  WWWWWWWWWWWWWWWW   ",
    " WWWWWWWWWWWWWWWWWW  ",
    "WWWWWWWWWWWWWWWWWWWW ",
    "WWWWWWWWWWWWWWWWWWWW ",
    "WSSSSSSSSSSSSSSSSSSW ",
    " SSSSSSSSSSSSSSSSSS  ",
    "                     "
};

void DrawPixelCloud(float startX, float startY, float scale) {
    Color shadowColor = {190, 210, 245, 255}; // Light periwinkle shadow
    for (int y = 0; y < CLOUD_HEIGHT; y++) {
        for (int x = 0; x < CLOUD_WIDTH; x++) {
            char p = cloudTemplate[y][x];
            if (p == 'W') {
                DrawRectangle(startX + x * scale, startY + y * scale, scale, scale, WHITE);
            } else if (p == 'S') {
                DrawRectangle(startX + x * scale, startY + y * scale, scale, scale, shadowColor);
            }
        }
    }
}

const int COIN_WIDTH = 14;
const int COIN_HEIGHT = 16;
const char* coinTemplate[COIN_HEIGHT] = {
    "    YYYYYY    ",
    "  YYYYYYYYYY  ",
    " YYYYYYYYYYYY ",
    " YYYYIIYYYYYY ",
    "YYYYIIIIYYYYYY",
    "YYYYIIIIYYYYYY",
    "YYYYIIIIYYYYYY",
    "YYYYIIIIYYYYYY",
    "YYYYIIIIYYYYYY",
    "YYYYIIIIYYYYYY",
    "YYYYIIIIYYYYYY",
    "YYYYIIIIYYYYYY",
    " YYYYIIYYYYYY ",
    " YYYYYYYYYYYY ",
    "  YYYYYYYYYY  ",
    "    YYYYYY    "
};

void DrawPixelCoin(float startX, float startY, float scale) {
    Color gold = {255, 203, 0, 255};
    Color inner = {255, 245, 150, 255};
    for (int y = 0; y < COIN_HEIGHT; y++) {
        for (int x = 0; x < COIN_WIDTH; x++) {
            char p = coinTemplate[y][x];
            if (p == 'Y') {
                DrawRectangle(startX + x * scale, startY + y * scale, scale, scale, gold);
            } else if (p == 'I') {
                DrawRectangle(startX + x * scale, startY + y * scale, scale, scale, inner);
            }
        }
    }
}

void LoadLevel(Player& player) {
    enemies.clear();
    clouds.clear();
    player.score = 0;
    player.velocity = {0, 0};

    // Add random background clouds
    for (int i = 0; i < 12; i++) {
        Cloud c;
        c.position = { (float)GetRandomValue(-200, MAP_WIDTH * TILE_SIZE), (float)GetRandomValue(20, 250) };
        c.speed = (float)GetRandomValue(5, 20);
        c.scale = (float)GetRandomValue(3, 8); // Scale acts as pixel size now
        clouds.push_back(c);
    }

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (levelMap[y][x] == 'P') {
                player.position = { (float)x * TILE_SIZE, (float)y * TILE_SIZE };
                levelMap[y][x] = ' '; // Clear start marker
            }
            else if (levelMap[y][x] == 'E') {
                Enemy e;
                e.position = { (float)x * TILE_SIZE, (float)y * TILE_SIZE };
                e.velocity = { -100.0f, 0.0f }; // Start moving left
                e.isAlive = true;
                enemies.push_back(e);
                levelMap[y][x] = ' '; // Clear enemy marker
            }
        }
    }
}

// Reset level to original state (re-adding coins and enemies)
void ResetMap() {
    levelMap = {
        "################################################################################",
        "#                                                                              #",
        "#                                                                              #",
        "#                                                                              #",
        "#                                                                              #",
        "#                                                                              #",
        "#                                                                              #",
        "#                                         C C C                                #",
        "#                                        #######                               #",
        "#         C C C                 C                      E                       #",
        "#        #######               ####                 #######                    #",
        "#                                                                              #",
        "# P               E                                   C  C                   F #",
        "######################    ######################################################",
        "######################    ######################################################"
    };
}

// Check collision with solid tiles
bool IsSolid(int tileX, int tileY) {
    if (tileX < 0 || tileX >= MAP_WIDTH || tileY < 0 || tileY >= MAP_HEIGHT) return false;
    return levelMap[tileY][tileX] == '#';
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Super C++ Platformer");
    SetTargetFPS(60);
    SetExitKey(0);

    GameState currentState = MENU;
    Player player;

    Camera2D camera = { 0 };
    camera.target = { 0.0f, 0.0f };
    camera.offset = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // --- Input & Logic Update ---
        if (currentState == MENU) {
            if (IsKeyPressed(KEY_SPACE)) {
                ResetMap();
                LoadLevel(player);
                currentState = PLAYING;
            }
        }
        else if (currentState == GAME_OVER || currentState == LEVEL_COMPLETE) {
            if (IsKeyPressed(KEY_R)) {
                ResetMap();
                LoadLevel(player);
                currentState = PLAYING;
            }
            if (IsKeyPressed(KEY_Q)) {
                currentState = MENU;
            }
        }
        else if (currentState == PLAYING) {
            // -- Player Horizontal Movement --
            player.velocity.x = 0;
            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) player.velocity.x = -MOVE_SPEED;
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) player.velocity.x = MOVE_SPEED;

            // Apply horizontal velocity
            player.position.x += player.velocity.x * dt;

            // Horizontal Collision
            int minX = player.position.x / TILE_SIZE;
            int maxX = (player.position.x + TILE_SIZE - 1) / TILE_SIZE;
            int minY = player.position.y / TILE_SIZE;
            int maxY = (player.position.y + TILE_SIZE - 1) / TILE_SIZE;

            if (player.velocity.x > 0) { // Moving right
                if (IsSolid(maxX, minY) || IsSolid(maxX, maxY)) {
                    player.position.x = maxX * TILE_SIZE - TILE_SIZE;
                }
            } else if (player.velocity.x < 0) { // Moving left
                if (IsSolid(minX, minY) || IsSolid(minX, maxY)) {
                    player.position.x = (minX + 1) * TILE_SIZE;
                }
            }

            // -- Player Vertical Movement (Gravity & Jump) --
            player.velocity.y += GRAVITY * dt;
            player.isGrounded = false;

            if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
                // Check if standing on ground before jumping
                int bottomY = (player.position.y + TILE_SIZE) / TILE_SIZE;
                if (IsSolid(minX, bottomY) || IsSolid(maxX, bottomY)) {
                    player.velocity.y = JUMP_FORCE;
                }
            }

            // Apply vertical velocity
            player.position.y += player.velocity.y * dt;

            // Vertical Collision
            minX = player.position.x / TILE_SIZE;
            maxX = (player.position.x + TILE_SIZE - 1) / TILE_SIZE;
            minY = player.position.y / TILE_SIZE;
            maxY = (player.position.y + TILE_SIZE - 1) / TILE_SIZE;

            if (player.velocity.y > 0) { // Falling down
                if (IsSolid(minX, maxY) || IsSolid(maxX, maxY)) {
                    player.position.y = maxY * TILE_SIZE - TILE_SIZE;
                    player.velocity.y = 0;
                    player.isGrounded = true;
                }
            } else if (player.velocity.y < 0) { // Jumping up (hitting ceiling)
                if (IsSolid(minX, minY) || IsSolid(maxX, minY)) {
                    player.position.y = (minY + 1) * TILE_SIZE;
                    player.velocity.y = 0;
                }
            }

            // Check pit fall
            if (player.position.y > MAP_HEIGHT * TILE_SIZE) {
                currentState = GAME_OVER;
            }

            // -- Interactables --
            int centerTileX = (player.position.x + TILE_SIZE / 2) / TILE_SIZE;
            int centerTileY = (player.position.y + TILE_SIZE / 2) / TILE_SIZE;
            
            if (centerTileX >= 0 && centerTileX < MAP_WIDTH && centerTileY >= 0 && centerTileY < MAP_HEIGHT) {
                if (levelMap[centerTileY][centerTileX] == 'C') {
                    player.score += 100;
                    levelMap[centerTileY][centerTileX] = ' '; // Consume coin
                }
                if (levelMap[centerTileY][centerTileX] == 'F') {
                    currentState = LEVEL_COMPLETE;
                }
            }

            // -- Enemies --
            for (auto& e : enemies) {
                if (!e.isAlive) continue;

                // Move enemy
                e.velocity.y += GRAVITY * dt;
                e.position.x += e.velocity.x * dt;

                // Enemy Horizontal Collision
                int eMinX = e.position.x / TILE_SIZE;
                int eMaxX = (e.position.x + TILE_SIZE - 1) / TILE_SIZE;
                int eMinY = e.position.y / TILE_SIZE;
                int eMaxY = (e.position.y + TILE_SIZE - 1) / TILE_SIZE;

                if (e.velocity.x > 0 && (IsSolid(eMaxX, eMinY) || IsSolid(eMaxX, eMaxY))) {
                    e.position.x = eMaxX * TILE_SIZE - TILE_SIZE;
                    e.velocity.x *= -1; // Reverse direction
                } else if (e.velocity.x < 0 && (IsSolid(eMinX, eMinY) || IsSolid(eMinX, eMaxY))) {
                    e.position.x = (eMinX + 1) * TILE_SIZE;
                    e.velocity.x *= -1; // Reverse direction
                }
                
                // Ledge Detection (Don't fall off cliffs)
                int nextTileX = (e.position.x + (e.velocity.x > 0 ? TILE_SIZE : -1)) / TILE_SIZE;
                int bottomY = (e.position.y + TILE_SIZE) / TILE_SIZE;
                if (!IsSolid(nextTileX, bottomY)) {
                    e.velocity.x *= -1; // Turn around at ledges
                }

                // Enemy Vertical Collision
                e.position.y += e.velocity.y * dt;
                eMinX = e.position.x / TILE_SIZE;
                eMaxX = (e.position.x + TILE_SIZE - 1) / TILE_SIZE;
                eMinY = e.position.y / TILE_SIZE;
                eMaxY = (e.position.y + TILE_SIZE - 1) / TILE_SIZE;

                if (e.velocity.y > 0 && (IsSolid(eMinX, eMaxY) || IsSolid(eMaxX, eMaxY))) {
                    e.position.y = eMaxY * TILE_SIZE - TILE_SIZE;
                    e.velocity.y = 0;
                }

                // Check Player Collision with Enemy
                Rectangle playerRect = { player.position.x, player.position.y, TILE_SIZE, TILE_SIZE };
                Rectangle enemyRect = { e.position.x, e.position.y, TILE_SIZE, TILE_SIZE };

                if (CheckCollisionRecs(playerRect, enemyRect)) {
                    // If player is falling from above, kill enemy
                    if (player.velocity.y > 0 && player.position.y + TILE_SIZE - e.position.y < TILE_SIZE / 2) {
                        e.isAlive = false;
                        player.velocity.y = -400.0f; // Bop upwards
                        player.score += 200;
                    } else {
                        // Player got hit
                        currentState = GAME_OVER;
                    }
                }
            }

            // Update Camera to follow player smoothly
            camera.target.x = player.position.x + TILE_SIZE / 2;
            camera.target.y = player.position.y + TILE_SIZE / 2;

            // Clamp camera so we don't see out of bounds (left side)
            if (camera.target.x < SCREEN_WIDTH / 2) {
                camera.target.x = SCREEN_WIDTH / 2;
            }

            // -- Update Clouds --
            for (auto& c : clouds) {
                c.position.x += c.speed * dt;
                // If a cloud drifts off the right side of the map, wrap it to the left
                if (c.position.x > MAP_WIDTH * TILE_SIZE + 200) {
                    c.position.x = -200;
                    c.position.y = (float)GetRandomValue(50, 350);
                }
            }
            // Clamp camera so we don't see out of bounds (left side)
            if (camera.target.x < SCREEN_WIDTH / 2) {
                camera.target.x = SCREEN_WIDTH / 2;
            }
        }

        // --- Render ---
        BeginDrawing();
        
        // Draw retro dotted sky background
        ClearBackground({120, 160, 240, 255}); // Base sky blue
        for (int y = 0; y < SCREEN_HEIGHT; y += 20) {
            for (int x = 0; x < SCREEN_WIDTH; x += 20) {
                // Offset every other row to create a nice staggered dot pattern
                int offsetX = (y / 20) % 2 == 0 ? 0 : 10;
                DrawRectangle(x + offsetX, y, 4, 4, {100, 140, 220, 255}); // Darker blue dot
            }
        }

        if (currentState == MENU) {
            DrawText("SUPER C++ PLATFORMER", SCREEN_WIDTH/2 - MeasureText("SUPER C++ PLATFORMER", 40)/2, 200, 40, RAYWHITE);
            DrawText("Press SPACE to Start", SCREEN_WIDTH/2 - MeasureText("Press SPACE to Start", 20)/2, 350, 20, YELLOW);
        }
        else {
            BeginMode2D(camera);

            // Draw Clouds (background layer)
            for (auto& c : clouds) {
                DrawPixelCloud(c.position.x, c.position.y, c.scale);
            }

            // Draw Map
            for (int y = 0; y < MAP_HEIGHT; y++) {
                for (int x = 0; x < MAP_WIDTH; x++) {
                    char tile = levelMap[y][x];
                    if (tile == '#') {
                        DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, BROWN);
                        DrawRectangleLines(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, Fade(BLACK, 0.3f));
                    }
                    else if (tile == 'C') {
                        DrawPixelCoin(x * TILE_SIZE + 6, y * TILE_SIZE + 4, 2.0f);
                    }
                    else if (tile == 'F') {
                        DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE/4, TILE_SIZE * 3, LIGHTGRAY); // Flag pole
                        DrawTriangle({(float)x * TILE_SIZE + TILE_SIZE/4, (float)y * TILE_SIZE},
                                     {(float)x * TILE_SIZE + TILE_SIZE/4, (float)y * TILE_SIZE + TILE_SIZE},
                                     {(float)x * TILE_SIZE + TILE_SIZE, (float)y * TILE_SIZE + TILE_SIZE/2}, GREEN);
                    }
                }
            }

            // Draw Enemies
            for (auto& e : enemies) {
                if (e.isAlive) {
                    DrawRectangle(e.position.x, e.position.y, TILE_SIZE, TILE_SIZE, RED); // Goombas are red squares
                    DrawRectangle(e.position.x + 5, e.position.y + 10, 5, 5, BLACK); // Eyes
                    DrawRectangle(e.position.x + 30, e.position.y + 10, 5, 5, BLACK);
                }
            }

            // Draw Player
            DrawRectangle(player.position.x, player.position.y, TILE_SIZE, TILE_SIZE, BLUE);

            EndMode2D();

            // Draw UI (Un-affected by camera)
            DrawText(TextFormat("SCORE: %06d", player.score), 20, 20, 20, BLACK);

            if (currentState == GAME_OVER) {
                DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.8f));
                DrawText("GAME OVER!", SCREEN_WIDTH/2 - MeasureText("GAME OVER!", 60)/2, 200, 60, RED);
                DrawText("Press [R] to Restart", SCREEN_WIDTH/2 - MeasureText("Press [R] to Restart", 20)/2, 350, 20, RAYWHITE);
                DrawText("Press [Q] to Quit to Menu", SCREEN_WIDTH/2 - MeasureText("Press [Q] to Quit to Menu", 20)/2, 400, 20, GRAY);
            }
            else if (currentState == LEVEL_COMPLETE) {
                DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.8f));
                DrawText("LEVEL CLEAR!", SCREEN_WIDTH/2 - MeasureText("LEVEL CLEAR!", 60)/2, 200, 60, GREEN);
                DrawText(TextFormat("Final Score: %d", player.score), SCREEN_WIDTH/2 - MeasureText(TextFormat("Final Score: %d", player.score), 30)/2, 300, 30, YELLOW);
                DrawText("Press [R] to Play Again", SCREEN_WIDTH/2 - MeasureText("Press [R] to Play Again", 20)/2, 400, 20, RAYWHITE);
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
