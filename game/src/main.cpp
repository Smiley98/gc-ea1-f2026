#include "raylib.h"
#include "raymath.h"
#include "raygui.h"

int main(void)
{
    InitWindow(1280, 720, "Game");
    SetTargetFPS(60);

    {
        Vector2 a = Vector2Rotate(Vector2UnitX, 205.0f * DEG2RAD);
        Vector2 b = Vector2Rotate(Vector2UnitX, 285.0f * DEG2RAD);
        float dot = Vector2DotProduct(a, b);
        float ma = Vector2Length(a);
        float mb = Vector2Length(b);
        float cos = dot / (ma * mb);
        float theta = acosf(cos) * RAD2DEG;
        TraceLog(LOG_INFO, "lit");
    }

    while (!WindowShouldClose())
    {
        // Make GUI so students can check their answers!?
        BeginDrawing();
            ClearBackground(RAYWHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
