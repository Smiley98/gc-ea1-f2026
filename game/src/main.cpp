#include "raylib.h"
#include "raymath.h"
#include "raygui.h"

int main()
{
    InitWindow(800, 800, "Game");
    InitAudioDevice();
    SetTargetFPS(60);

    Vector2 player_direction = Vector2UnitX;
    Vector2 player_position = { GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f };

    Vector2 bullet_position = player_position;
    Vector2 bullet_velocity = Vector2Zeros;
    float bullet_speed = 250.0f;

    while (!WindowShouldClose())
    {
        if (IsKeyDown(KEY_A))
        {
            player_direction = Vector2UnitX * -1.0f;
        }
        else if (IsKeyDown(KEY_D))
        {
            player_direction = Vector2UnitX;
        }

        if (IsKeyPressed(KEY_SPACE))
        {
            bullet_position = player_position;
            bullet_velocity = player_direction * bullet_speed;
        }

        float dt = GetFrameTime();
        bullet_position += bullet_velocity * dt;

        BeginDrawing();
        ClearBackground(WHITE);
        DrawCircleV(bullet_position, 10.0f, RED);
        DrawCircleV(player_position, 20.0f, BLUE);
        DrawLineEx(player_position, player_position + player_direction * 100.0f, 4.0f, SKYBLUE);

        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
}
