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
const int SCREEN_WIDTH = BOARD_WIDTH * CELL_SIZE + 200; // Extra space for side panel
const int SCREEN_HEIGHT = BOARD_HEIGHT * CELL_SIZE;

// Game states
enum GameState {
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER
};

// --- Tetromino Definitions ---
// Each tetromino is a 4x4 grid of cells. We store all 7 shapes.
// 0=empty, 1=filled
const int TETROMINO_COUNT = 7;

// Shape data: each shape has 4 rotations, each rotation is 4x4
struct Tetromino {
    int cells[4][4];
    Color color;
};

// All 7 tetrominoes (in their default rotation)
Tetromino tetrominoes[TETROMINO_COUNT] = {
    // I-piece
    {{{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}}, SKYBLUE},
    // O-piece
    {{{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}}, YELLOW},
    // T-piece
    {{{0,0,0,0},{1,1,1,0},{0,1,0,0},{0,0,0,0}}, PURPLE},
    // S-piece
    {{{0,0,0,0},{0,1,1,0},{1,1,0,0},{0,0,0,0}}, GREEN},
    // Z-piece
    {{{0,0,0,0},{1,1,0,0},{0,1,1,0},{0,0,0,0}}, RED},
    // J-piece
    {{{0,0,0,0},{1,1,1,0},{0,0,1,0},{0,0,0,0}}, DARKBLUE},
    // L-piece
    {{{0,0,0,0},{1,1,1,0},{1,0,0,0},{0,0,0,0}}, ORANGE}
};

// --- Game Board ---
// The board stores the color index of locked pieces (0 = empty, 1-7 = piece type)
int board[BOARD_WIDTH][BOARD_HEIGHT] = {0};
Color boardColors[BOARD_WIDTH][BOARD_HEIGHT];

// --- Current Piece State ---
Tetromino currentPiece;
int currentType = 0;
int currentX = 3;
int currentY = 0;

int nextType = 0;

// --- Rotation Function ---
// Rotates a 4x4 tetromino grid 90 degrees clockwise
Tetromino RotatePiece(Tetromino piece) {
    Tetromino rotated = piece;
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            rotated.cells[x][3 - y] = piece.cells[y][x];
        }
    }
    return rotated;
}

// --- Collision Detection ---
bool CheckCollision(Tetromino piece, int posX, int posY) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (piece.cells[y][x] != 0) {
                int boardX = posX + x;
                int boardY = posY + y;

                // Check bounds
                if (boardX < 0 || boardX >= BOARD_WIDTH || boardY >= BOARD_HEIGHT) {
                    return true;
                }
                // Check against locked pieces (only if on the board)
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

// --- Clear Completed Lines ---
int ClearLines() {
    int linesCleared = 0;
    for (int y = BOARD_HEIGHT - 1; y >= 0; y--) {
        bool full = true;
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (board[x][y] == 0) {
                full = false;
                break;
            }
        }
        if (full) {
            linesCleared++;
            // Shift everything above this line down by one
            for (int row = y; row > 0; row--) {
                for (int x = 0; x < BOARD_WIDTH; x++) {
                    board[x][row] = board[x][row - 1];
                    boardColors[x][row] = boardColors[x][row - 1];
                }
            }
            // Clear the top row
            for (int x = 0; x < BOARD_WIDTH; x++) {
                board[x][0] = 0;
            }
            // Re-check the same row since everything shifted down
            y++;
        }
    }
    return linesCleared;
}

// --- Spawn New Piece ---
void SpawnPiece() {
    currentType = nextType;
    currentPiece = tetrominoes[currentType];
    currentX = 3;
    currentY = 0;
    nextType = GetRandomValue(0, TETROMINO_COUNT - 1);
}

// --- Reset Game ---
void ResetGame(int& score, int& level, int& totalLines) {
    for (int x = 0; x < BOARD_WIDTH; x++) {
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            board[x][y] = 0;
        }
    }
    score = 0;
    level = 1;
    totalLines = 0;
    nextType = GetRandomValue(0, TETROMINO_COUNT - 1);
    SpawnPiece();
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tetris!");
    InitAudioDevice();
    SetTargetFPS(60);
    SetExitKey(0);

    GameState currentState = MENU;
    int score = 0;
    int level = 1;
    int totalLines = 0;

    float dropTimer = 0.0f;
    float dropInterval = 0.5f; // Seconds between automatic drops

    nextType = GetRandomValue(0, TETROMINO_COUNT - 1);
    SpawnPiece();

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // --- Logic ---
        if (currentState == MENU) {
            if (IsKeyPressed(KEY_SPACE)) {
                ResetGame(score, level, totalLines);
                currentState = PLAYING;
            }
        }
        else if (currentState == GAME_OVER) {
            if (IsKeyPressed(KEY_R)) {
                ResetGame(score, level, totalLines);
                currentState = PLAYING;
            }
            if (IsKeyPressed(KEY_Q)) {
                currentState = MENU;
            }
        }
        else if (currentState == PAUSED) {
            if (IsKeyPressed(KEY_P)) currentState = PLAYING;
            if (IsKeyPressed(KEY_Q)) currentState = MENU;
        }
        else if (currentState == PLAYING) {
            // --- Input ---
            if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
                if (!CheckCollision(currentPiece, currentX - 1, currentY)) {
                    currentX--;
                }
            }
            if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
                if (!CheckCollision(currentPiece, currentX + 1, currentY)) {
                    currentX++;
                }
            }
            if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
                if (!CheckCollision(currentPiece, currentX, currentY + 1)) {
                    currentY++;
                    score += 1; // Soft drop bonus
                }
            }
            // Hard drop
            if (IsKeyPressed(KEY_SPACE)) {
                while (!CheckCollision(currentPiece, currentX, currentY + 1)) {
                    currentY++;
                    score += 2; // Hard drop bonus
                }
                // Lock immediately
                LockPiece(currentPiece, currentX, currentY);
                int cleared = ClearLines();
                if (cleared > 0) {
                    // Scoring: 100, 300, 500, 800 for 1-4 lines
                    int lineScores[] = {0, 100, 300, 500, 800};
                    score += lineScores[cleared] * level;
                    totalLines += cleared;
                    level = 1 + totalLines / 10;
                    dropInterval = 0.5f - (level - 1) * 0.05f;
                    if (dropInterval < 0.05f) dropInterval = 0.05f;
                }
                SpawnPiece();
                if (CheckCollision(currentPiece, currentX, currentY)) {
                    currentState = GAME_OVER;
                }
                dropTimer = 0.0f;
                goto render; // Skip normal drop this frame
            }
            // Rotate
            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
                Tetromino rotated = RotatePiece(currentPiece);
                if (!CheckCollision(rotated, currentX, currentY)) {
                    currentPiece = rotated;
                }
                // Wall kick: try shifting left or right if rotation fails at the wall
                else if (!CheckCollision(rotated, currentX - 1, currentY)) {
                    currentX--;
                    currentPiece = rotated;
                }
                else if (!CheckCollision(rotated, currentX + 1, currentY)) {
                    currentX++;
                    currentPiece = rotated;
                }
            }

            // Pause / Quit
            if (IsKeyPressed(KEY_P)) { currentState = PAUSED; goto render; }
            if (IsKeyPressed(KEY_Q)) { currentState = MENU; goto render; }

            // --- Gravity ---
            dropTimer += dt;
            if (dropTimer >= dropInterval) {
                dropTimer = 0.0f;
                if (!CheckCollision(currentPiece, currentX, currentY + 1)) {
                    currentY++;
                } else {
                    // Lock the piece
                    LockPiece(currentPiece, currentX, currentY);
                    int cleared = ClearLines();
                    if (cleared > 0) {
                        int lineScores[] = {0, 100, 300, 500, 800};
                        score += lineScores[cleared] * level;
                        totalLines += cleared;
                        level = 1 + totalLines / 10;
                        dropInterval = 0.5f - (level - 1) * 0.05f;
                        if (dropInterval < 0.05f) dropInterval = 0.05f;
                    }
                    SpawnPiece();
                    if (CheckCollision(currentPiece, currentX, currentY)) {
                        currentState = GAME_OVER;
                    }
                }
            }
        }

        render:
        // --- Render ---
        BeginDrawing();
        ClearBackground(DARKGRAY);

        if (currentState == MENU) {
            DrawText("TETRIS", SCREEN_WIDTH/2 - MeasureText("TETRIS", 60)/2, 100, 60, RAYWHITE);
            
            DrawText("Controls:", 120, 280, 20, LIGHTGRAY);
            DrawText("Move: A / D  or  Left / Right", 120, 310, 18, GRAY);
            DrawText("Soft Drop: S  or  Down", 120, 340, 18, GRAY);
            DrawText("Hard Drop: Space", 120, 370, 18, GRAY);
            DrawText("Rotate: W  or  Up", 120, 400, 18, GRAY);
            DrawText("Pause: P  |  Quit: Q", 120, 430, 18, GRAY);

            DrawText("Press SPACE to Start", SCREEN_WIDTH/2 - MeasureText("Press SPACE to Start", 30)/2, 510, 30, RAYWHITE);
        }
        else if (currentState == PLAYING || currentState == PAUSED) {
            // Draw Board Background
            DrawRectangle(0, 0, BOARD_WIDTH * CELL_SIZE, BOARD_HEIGHT * CELL_SIZE, BLACK);

            // Draw Grid Lines
            for (int x = 0; x <= BOARD_WIDTH; x++) {
                DrawLine(x * CELL_SIZE, 0, x * CELL_SIZE, BOARD_HEIGHT * CELL_SIZE, Fade(GRAY, 0.3f));
            }
            for (int y = 0; y <= BOARD_HEIGHT; y++) {
                DrawLine(0, y * CELL_SIZE, BOARD_WIDTH * CELL_SIZE, y * CELL_SIZE, Fade(GRAY, 0.3f));
            }

            // Draw Locked Pieces
            for (int x = 0; x < BOARD_WIDTH; x++) {
                for (int y = 0; y < BOARD_HEIGHT; y++) {
                    if (board[x][y] != 0) {
                        DrawRectangle(x * CELL_SIZE + 1, y * CELL_SIZE + 1, CELL_SIZE - 2, CELL_SIZE - 2, boardColors[x][y]);
                    }
                }
            }

            // Draw Current Piece
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

            // Draw Ghost Piece (preview of where the piece will land)
            int ghostY = currentY;
            while (!CheckCollision(currentPiece, currentX, ghostY + 1)) {
                ghostY++;
            }
            if (ghostY != currentY) {
                for (int y = 0; y < 4; y++) {
                    for (int x = 0; x < 4; x++) {
                        if (currentPiece.cells[y][x] != 0) {
                            int drawX = (currentX + x) * CELL_SIZE;
                            int drawY = (ghostY + y) * CELL_SIZE;
                            if (drawY >= 0) {
                                DrawRectangle(drawX + 1, drawY + 1, CELL_SIZE - 2, CELL_SIZE - 2, Fade(currentPiece.color, 0.3f));
                            }
                        }
                    }
                }
            }

            // --- Side Panel ---
            int panelX = BOARD_WIDTH * CELL_SIZE + 20;

            DrawText("TETRIS", panelX, 10, 20, RAYWHITE);
            DrawText(TextFormat("Score: %d", score), panelX, 50, 20, LIGHTGRAY);
            DrawText(TextFormat("Level: %d", level), panelX, 80, 20, LIGHTGRAY);
            DrawText(TextFormat("Lines: %d", totalLines), panelX, 110, 20, LIGHTGRAY);

            // Draw Next Piece Preview
            DrawText("Next:", panelX, 170, 20, LIGHTGRAY);
            Tetromino nextPiece = tetrominoes[nextType];
            for (int y = 0; y < 4; y++) {
                for (int x = 0; x < 4; x++) {
                    if (nextPiece.cells[y][x] != 0) {
                        DrawRectangle(panelX + x * 25, 200 + y * 25, 23, 23, nextPiece.color);
                    }
                }
            }

            // Pause Overlay
            if (currentState == PAUSED) {
                DrawRectangle(0, 0, BOARD_WIDTH * CELL_SIZE, BOARD_HEIGHT * CELL_SIZE, Fade(BLACK, 0.7f));
                DrawText("PAUSED", BOARD_WIDTH * CELL_SIZE / 2 - MeasureText("PAUSED", 40)/2, 250, 40, RAYWHITE);
                DrawText("[P] Resume  [Q] Quit", BOARD_WIDTH * CELL_SIZE / 2 - MeasureText("[P] Resume  [Q] Quit", 18)/2, 300, 18, GRAY);
            }
        }
        else if (currentState == GAME_OVER) {
            DrawText("GAME OVER", SCREEN_WIDTH/2 - MeasureText("GAME OVER", 60)/2, 150, 60, RED);
            DrawText(TextFormat("Final Score: %d", score), SCREEN_WIDTH/2 - MeasureText(TextFormat("Final Score: %d", score), 30)/2, 240, 30, LIGHTGRAY);
            DrawText(TextFormat("Level: %d  |  Lines: %d", level, totalLines), SCREEN_WIDTH/2 - MeasureText(TextFormat("Level: %d  |  Lines: %d", level, totalLines), 20)/2, 290, 20, GRAY);

            DrawText("[R] Restart  [Q] Menu", SCREEN_WIDTH/2 - MeasureText("[R] Restart  [Q] Menu", 20)/2, 400, 20, DARKGRAY);
        }

        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
}
