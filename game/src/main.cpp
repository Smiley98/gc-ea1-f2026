#include "raylib.h"
#include "raymath.h"
#include "raygui.h"

int main()
{
    InitWindow(800, 800, "Game");
    InitAudioDevice();
    SetTargetFPS(60);

    Vector2 plane_position = { 400.0f, 400.0f };
    Vector2 plane_normal = Vector2UnitY * -1.0f;
    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        if (IsKeyDown(KEY_Q))
        {
            plane_normal = Vector2Rotate(plane_normal, -100.0f * DEG2RAD * dt);
        }
        if (IsKeyDown(KEY_E))
        {
            plane_normal = Vector2Rotate(plane_normal, 100.0f * DEG2RAD * dt);
        }

        Vector2 circle_position = GetMousePosition();
        float circle_radius = 20.0f;

        // AB = B - A (FROM plane TO circle)
        Vector2 to_circle = circle_position - plane_position;
        float scalar_proj = Vector2DotProduct(to_circle, plane_normal);

        // Collision if scalar projection is less than or equal to the radius of the circle

        BeginDrawing();
        ClearBackground(WHITE);

        Vector2 plane_surface = { plane_normal.y, -plane_normal.x };
        DrawLineEx(plane_position + plane_surface * 100.0f, plane_position - plane_surface * 100.0f, 6.0f, GREEN);
        DrawLineEx(plane_position, plane_position + plane_normal * 50.0f, 4.0f, SKYBLUE);
        DrawCircleV(circle_position, circle_radius, BLUE);
        DrawLineV(plane_position, plane_position + to_circle, DARKBLUE);
        DrawLineV(plane_position, plane_position + plane_normal * scalar_proj, PURPLE);
        DrawCircleV(plane_position + plane_normal * scalar_proj, 10.0f, MAGENTA);

        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
}
