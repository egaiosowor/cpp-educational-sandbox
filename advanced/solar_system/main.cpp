#include "raylib.h"
#include <cmath>
#include <vector>
#include <string>

struct Planet {
    std::string name;
    float radius;
    float distance; // Distance from sun
    float speed;    // Orbital speed multiplier
    float angle;    // Current angle
    Color color;
    bool hasRings;
};

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "3D Full Solar System Viewer");

    // Initialize 3D Camera (Pulled back to see everything)
    Camera3D camera = { 0 };
    camera.position = { 30.0f, 40.0f, 40.0f };  
    camera.target = { 0.0f, 0.0f, 0.0f };       
    camera.up = { 0.0f, 1.0f, 0.0f };           
    camera.fovy = 45.0f;                        
    camera.projection = CAMERA_PERSPECTIVE;     

    std::vector<Planet> planets = {
        {"Mercury", 0.3f, 4.0f, 2.0f, 0.0f, DARKGRAY, false},
        {"Venus",   0.5f, 6.0f, 1.5f, 0.0f, ORANGE, false},
        {"Earth",   0.6f, 9.0f, 1.0f, 0.0f, BLUE, false},
        {"Mars",    0.4f, 12.0f, 0.8f, 0.0f, RED, false},
        {"Jupiter", 2.0f, 18.0f, 0.4f, 0.0f, BROWN, false},
        {"Saturn",  1.6f, 24.0f, 0.3f, 0.0f, GOLD, true},
        {"Uranus",  1.0f, 29.0f, 0.2f, 0.0f, SKYBLUE, false},
        {"Neptune", 1.0f, 34.0f, 0.1f, 0.0f, DARKBLUE, false}
    };

    float moonOrbitAngle = 0.0f;

    // Spherical Coordinate Camera variables
    float camRadius = 60.0f;
    float camTheta = 0.7f;
    float camPhi = 0.5f;

    SetTargetFPS(60);

    // Lock the cursor to the window so moving the mouse freely rotates the camera infinitely
    DisableCursor();

    while (!WindowShouldClose()) {
        // --- Logic ---
        float dt = GetFrameTime();
        
        // Update angles
        for (auto& p : planets) {
            p.angle += p.speed * dt;
        }
        moonOrbitAngle += 3.0f * dt;

        // --- Camera Controls (Spherical Math) ---
        // Rotate with mouse movement (Cursor is locked to window)
        Vector2 delta = GetMouseDelta();
        camTheta -= delta.x * 0.005f;
        camPhi -= delta.y * 0.005f;

        // Clamp camera angles to prevent flipping upside down
        if (camPhi < 0.1f) camPhi = 0.1f;
        if (camPhi > 3.14159f - 0.1f) camPhi = 3.14159f - 0.1f;

        // Zoom with scroll wheel
        camRadius -= GetMouseWheelMove() * 5.0f;
        if (camRadius < 5.0f) camRadius = 5.0f;
        if (camRadius > 150.0f) camRadius = 150.0f;

        // Convert Spherical to Cartesian Coordinates
        camera.position.x = camRadius * std::sin(camPhi) * std::cos(camTheta);
        camera.position.z = camRadius * std::sin(camPhi) * std::sin(camTheta);
        camera.position.y = camRadius * std::cos(camPhi);

        // --- Render ---
        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(camera);

            // Draw Grid (Extended size)
            DrawGrid(40, 2.0f);

            // Draw Sun (Center)
            DrawSphere({ 0.0f, 0.0f, 0.0f }, 2.5f, YELLOW);
            DrawSphereWires({ 0.0f, 0.0f, 0.0f }, 2.5f, 16, 16, ORANGE);

            // Draw Planets
            for (size_t i = 0; i < planets.size(); i++) {
                // Trigonometry to find X and Z position based on angle and distance
                float pX = std::cos(planets[i].angle) * planets[i].distance;
                float pZ = std::sin(planets[i].angle) * planets[i].distance;
                Vector3 pPos = { pX, 0.0f, pZ };

                // Draw Orbital Path
                DrawCircle3D({0, 0, 0}, planets[i].distance, {1, 0, 0}, 90.0f, Fade(DARKGRAY, 0.5f));

                // Draw Planet Body
                DrawSphere(pPos, planets[i].radius, planets[i].color);
                DrawSphereWires(pPos, planets[i].radius, 16, 16, Fade(BLACK, 0.3f));

                // If Saturn, draw rings
                if (planets[i].hasRings) {
                    DrawCircle3D(pPos, planets[i].radius + 1.5f, {1, 0, 0}, 90.0f, Fade(LIGHTGRAY, 0.8f));
                    DrawCircle3D(pPos, planets[i].radius + 2.0f, {1, 0, 0}, 90.0f, Fade(GRAY, 0.8f));
                }

                // If Earth, calculate and draw Moon! (Earth is index 2)
                if (i == 2) {
                    float mX = pX + std::cos(moonOrbitAngle) * 1.5f;
                    float mZ = pZ + std::sin(moonOrbitAngle) * 1.5f;
                    Vector3 mPos = { mX, 0.0f, mZ };
                    DrawSphere(mPos, 0.2f, LIGHTGRAY);
                }
            }

        EndMode3D();

        // UI Overlay
        DrawText("3D Full Solar System Viewer", 10, 10, 20, RAYWHITE);
        DrawText("Move Mouse to Orbit | Scroll/Pinch to Zoom | Press ESC to Exit", 10, 40, 20, YELLOW);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
