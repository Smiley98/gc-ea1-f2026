#include "raylib.h"
#include "raymath.h"
#include "raygui.h"

int main()
{
    InitWindow(800, 800, "Game");
    InitAudioDevice();
    SetTargetFPS(60);

    int scene = 0;

    float ball_radius = 20.0f;
    Vector2 ball_pos = { 100.0f, 400.0f };
    Vector2 ball_vel = Vector2UnitX * 250.0f;
    Color ball_color = RED;
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_TAB))
        {
            ++scene %= 6;
            if (scene == 0)
            {
                ball_color = RED;
                ball_pos = { 100.0f, 400.0f };
                ball_vel = Vector2UnitX * 250.0f;
            }
            else if (scene == 1)
            {
                ball_color = GREEN;
                ball_pos = { 400.0f, 100.0f };
                ball_vel = Vector2UnitY * 250.0f;
            }
            else if (scene == 2)
            {
                ball_color = BLUE;
            }
            else if (scene == 3)
            {
                ball_color = PURPLE;
            }
            else if (scene == 4)
            {
                ball_color = ORANGE;
            }
            else if (scene == 5)
            {
                ball_color = SKYBLUE;
                ball_vel = Vector2Normalize(Vector2UnitX + Vector2UnitY * 0.5f) * 250.0f;
                ball_pos = { 400.0f, 400.0f };
            }
        }

        float dt = GetFrameTime();
        if (scene == 0)
        {
            ball_pos += ball_vel * dt;
            if (ball_pos.x - ball_radius >= GetScreenWidth()) ball_pos.x = 0.0f;
        }
        else if (scene == 1)
        {
            ball_pos += ball_vel * dt;
            if (ball_pos.y - ball_radius >= GetScreenHeight()) ball_pos.y = 0.0f;
        }
        else if (scene == 2)
        {
            ball_pos.x = 400.0f + sinf(GetTime()) * 300.0f;
            ball_pos.y = 400.0f;
        }
        else if (scene == 3)
        {
            ball_pos.x = 400.0f;
            ball_pos.y = 400.0f + sinf(GetTime()) * 300.0f;
        }
        else if (scene == 4)
        {
            ball_pos.x = 400.0f + cosf(GetTime()) * 300.0f;
            ball_pos.y = 400.0f + sinf(GetTime()) * 300.0f;
        }
        else if (scene == 5)
        {
            Vector2 next_pos = ball_pos + ball_vel * dt;
            if (next_pos.x + ball_radius >= GetScreenWidth()  || next_pos.x - ball_radius <= 0.0f) ball_vel.x *= -1.0f;
            if (next_pos.y + ball_radius >= GetScreenHeight() || next_pos.y - ball_radius <= 0.0f) ball_vel.y *= -1.0f;
            ball_pos += ball_vel * dt;
        }

        BeginDrawing();
        ClearBackground(WHITE);
        DrawCircleV(ball_pos, ball_radius, ball_color);
        if (scene == 5) DrawText("DVD", ball_pos.x - ball_radius, ball_pos.y - 10, 20, DARKBLUE);
        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
}
