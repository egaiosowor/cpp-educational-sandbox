#include "raylib.h"
#include <vector>
#include <array>
#include <cstdlib>
#include <ctime>
#include <string>

// --- Constants ---
const int CELL_SIZE = 30;
const int BOARD_WIDTH = 10;
const int BOARD_HEIGHT = 20;
const int SCREEN_WIDTH = BOARD_WIDTH * CELL_SIZE + 200;
const int SCREEN_HEIGHT = BOARD_HEIGHT * CELL_SIZE;

// Game states
enum GameState {
    MENU,
    PLAYING,
    GAME_OVER
};

// --- Tetromino Definitions ---
const int TETROMINO_COUNT = 7;

struct Tetromino {
    int cells[4][4];
    Color color;
};

Tetromino tetrominoes[TETROMINO_COUNT] = {
    {{{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}}, SKYBLUE},
    {{{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}}, YELLOW},
    {{{0,0,0,0},{1,1,1,0},{0,1,0,0},{0,0,0,0}}, PURPLE},
    {{{0,0,0,0},{0,1,1,0},{1,1,0,0},{0,0,0,0}}, GREEN},
    {{{0,0,0,0},{1,1,0,0},{0,1,1,0},{0,0,0,0}}, RED},
    {{{0,0,0,0},{1,1,1,0},{0,0,1,0},{0,0,0,0}}, DARKBLUE},
    {{{0,0,0,0},{1,1,1,0},{1,0,0,0},{0,0,0,0}}, ORANGE}
};

// --- Game Board ---
int board[BOARD_WIDTH][BOARD_HEIGHT] = {0};
Color boardColors[BOARD_WIDTH][BOARD_HEIGHT];

// --- Current Piece State ---
Tetromino currentPiece;
int currentType = 0;
int currentX = 3;
int currentY = 0;

// --- Collision Detection ---
bool CheckCollision(Tetromino piece, int posX, int posY) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (piece.cells[y][x] != 0) {
                int boardX = posX + x;
                int boardY = posY + y;
                if (boardX < 0 || boardX >= BOARD_WIDTH || boardY >= BOARD_HEIGHT) {
                    return true;
                }
                if (boardY >= 0 && board[boardX][boardY] != 0) {
                    return true;
                }
            }
        }
    }
    return false;
}

// --- Lock Piece to Board ---
void LockPiece(Tetromino piece, int posX, int posY) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (piece.cells[y][x] != 0) {
                int boardX = posX + x;
                int boardY = posY + y;
                if (boardX >= 0 && boardX < BOARD_WIDTH && boardY >= 0 && boardY < BOARD_HEIGHT) {
                    board[boardX][boardY] = currentType + 1;
                    boardColors[boardX][boardY] = piece.color;
                }
            }
        }
    }
}

void SpawnPiece() {
    currentType = GetRandomValue(0, TETROMINO_COUNT - 1);
    currentPiece = tetrominoes[currentType];
    currentX = 3;
    currentY = 0;
}

void ResetGame() {
    for (int x = 0; x < BOARD_WIDTH; x++) {
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            board[x][y] = 0;
        }
    }
    SpawnPiece();
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tetris!");
    SetTargetFPS(60);
    SetExitKey(0);

    GameState currentState = MENU;
    float dropTimer = 0.0f;
    float dropInterval = 0.5f;

    SpawnPiece();

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        if (currentState == MENU) {
            if (IsKeyPressed(KEY_SPACE)) {
                ResetGame();
                currentState = PLAYING;
            }
        }
        else if (currentState == GAME_OVER) {
            if (IsKeyPressed(KEY_R)) {
                ResetGame();
                currentState = PLAYING;
            }
            if (IsKeyPressed(KEY_Q)) currentState = MENU;
        }
        else if (currentState == PLAYING) {
            // Movement
            if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
                if (!CheckCollision(currentPiece, currentX - 1, currentY)) currentX--;
            }
            if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
                if (!CheckCollision(currentPiece, currentX + 1, currentY)) currentX++;
            }
            if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
                if (!CheckCollision(currentPiece, currentX, currentY + 1)) currentY++;
            }
            if (IsKeyPressed(KEY_Q)) { currentState = MENU; }

            // Gravity
            dropTimer += dt;
            if (dropTimer >= dropInterval) {
                dropTimer = 0.0f;
                if (!CheckCollision(currentPiece, currentX, currentY + 1)) {
                    currentY++;
                } else {
                    LockPiece(currentPiece, currentX, currentY);
                    SpawnPiece();
                    if (CheckCollision(currentPiece, currentX, currentY)) {
                        currentState = GAME_OVER;
                    }
                }
            }
        }

        // --- Render ---
        BeginDrawing();
        ClearBackground(DARKGRAY);

        if (currentState == MENU) {
            DrawText("TETRIS", SCREEN_WIDTH/2 - MeasureText("TETRIS", 60)/2, 200, 60, RAYWHITE);
            DrawText("Press SPACE to Start", SCREEN_WIDTH/2 - MeasureText("Press SPACE to Start", 30)/2, 400, 30, RAYWHITE);
        }
        else if (currentState == PLAYING) {
            DrawRectangle(0, 0, BOARD_WIDTH * CELL_SIZE, BOARD_HEIGHT * CELL_SIZE, BLACK);

            for (int x = 0; x <= BOARD_WIDTH; x++)
                DrawLine(x * CELL_SIZE, 0, x * CELL_SIZE, BOARD_HEIGHT * CELL_SIZE, Fade(GRAY, 0.3f));
            for (int y = 0; y <= BOARD_HEIGHT; y++)
                DrawLine(0, y * CELL_SIZE, BOARD_WIDTH * CELL_SIZE, y * CELL_SIZE, Fade(GRAY, 0.3f));

            for (int x = 0; x < BOARD_WIDTH; x++) {
                for (int y = 0; y < BOARD_HEIGHT; y++) {
                    if (board[x][y] != 0) {
                        DrawRectangle(x * CELL_SIZE + 1, y * CELL_SIZE + 1, CELL_SIZE - 2, CELL_SIZE - 2, boardColors[x][y]);
                    }
                }
            }

            for (int y = 0; y < 4; y++) {
                for (int x = 0; x < 4; x++) {
                    if (currentPiece.cells[y][x] != 0) {
                        int drawX = (currentX + x) * CELL_SIZE;
                        int drawY = (currentY + y) * CELL_SIZE;
                        if (drawY >= 0) {
                            DrawRectangle(drawX + 1, drawY + 1, CELL_SIZE - 2, CELL_SIZE - 2, currentPiece.color);
                        }
                    }
                }
            }

            DrawText("TETRIS", BOARD_WIDTH * CELL_SIZE + 20, 10, 20, RAYWHITE);
        }
        else if (currentState == GAME_OVER) {
            DrawText("GAME OVER", SCREEN_WIDTH/2 - MeasureText("GAME OVER", 60)/2, 200, 60, RED);
            DrawText("[R] Restart  [Q] Menu", SCREEN_WIDTH/2 - MeasureText("[R] Restart  [Q] Menu", 20)/2, 350, 20, DARKGRAY);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
