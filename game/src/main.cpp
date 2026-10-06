#include "raylib.h"
#include "raymath.h"
#include "raygui.h"

RMAPI Vector2 Vector2ProjectPointLine(Vector2 P, Vector2 A, Vector2 B)
{
    Vector2 AB = (B - A);
    float t = Vector2DotProduct((P - A), AB) / Vector2DotProduct(AB, AB);
    return A + AB * t;//(AB * Clamp(t, 0.0f, 1.0f));
}

int main()
{
    InitWindow(800, 800, "Game");
    InitAudioDevice();
    SetTargetFPS(60);

    Vector2 origin = { 400.0f, 400.0f };

    float angle_b = 0.0f;

    while (!WindowShouldClose())
    {
        Vector2 a = GetMousePosition();
        Vector2 b = Vector2Rotate(Vector2UnitX, angle_b * DEG2RAD) * 250.0f;
        //Vector2 proj = Vector2DotProduct(a, b) / Vector2DotProduct(b, b) * b;
        Vector2 proj = Vector2ProjectPointLine(a, origin, origin + b);

        BeginDrawing();
        ClearBackground(WHITE);

        DrawCircleV(a, 20.0f, RED);
        DrawLineEx(origin, a, 4.0f, ORANGE);
        DrawLineEx(origin, origin + b, 4.0f, BLUE);
        DrawCircleV(proj, 8.0f, PURPLE);
        DrawLineEx(a, proj, 4.0f, PINK);

        GuiSlider({ 20, 10, 160, 80 }, "0", "360", &angle_b, 0.0f, 360.0f);

        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
}
