#include "raylib.h"
#include "raymath.h"
#include "raygui.h"

#include <array>
#include <vector>
#include <algorithm>

constexpr float BALL_RADIUS = 25.0f;
constexpr Vector2 GRAVITY = { 0.0f, 100.0f };

struct Test
{
    bool enabled;
};

int main()
{
    InitWindow(800, 800, "Game");
    InitAudioDevice();
    SetTargetFPS(60);

    // Simple automatic erase example -- tests[0] is removed because its not enabled
    std::vector<Test> tests;
    tests.push_back({ false });
    tests.push_back({ true });
    tests.push_back({ true });
    std::erase_if(tests, [](Test test) { return !test.enabled; });

    Rectangle ground;
    ground.x = 0;
    ground.y = 700;
    ground.width = GetScreenWidth();
    ground.height = 20.0f;

    Rectangle platform;
    platform.x = 100.0f;
    platform.y = ground.y - 100.0f;
    platform.width = 50.0f;
    platform.height = 100.0f;

    Vector2 ball_launch_position;
    ball_launch_position.x = platform.x + BALL_RADIUS;
    ball_launch_position.y = platform.y - BALL_RADIUS;

    Vector2 ball_position = ball_launch_position;
    Vector2 ball_velocity = Vector2Zeros;
    float ball_gravity_scale = 0.0f;

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        if (IsKeyPressed(KEY_SPACE))
        {
            ball_velocity = Vector2Rotate(Vector2UnitX, -30.0f * DEG2RAD) * 200.0f;
            ball_gravity_scale = 1.0f;
        }

        ball_velocity += GRAVITY * dt * ball_gravity_scale;
        ball_position += ball_velocity * dt;

        

        BeginDrawing();
            ClearBackground(WHITE);
            DrawRectangleRec(ground, BEIGE);
            DrawRectangleRec(platform, GRAY);

            DrawCircleV(ball_position, BALL_RADIUS, DARKGRAY);

            DrawCircleV(GetMousePosition(), 20.0f, RED);
        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
}
