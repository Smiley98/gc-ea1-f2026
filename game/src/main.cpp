#include "raylib.h"
#include "raymath.h"
#include "raygui.h"

float AngleBetween(Vector3 a, Vector3 b)
{
    float dot = Vector3DotProduct(a, b);
    float ma = Vector3Length(a);
    float mb = Vector3Length(b);
    float cos = dot / (ma * mb);
    float theta = acosf(cos) * RAD2DEG;
    return theta;
}

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

    // Get students to build a math library cause I don't wanna teach matrices for more than 3 weeks?
    {
        float d = Vector3DotProduct({ 2, 4, 6 }, { -5, 7, 9 });
        float a1 = Vector3Angle({ 2, 4, 6 }, { -5, 7, 9 }) * RAD2DEG;
        float a2 = AngleBetween({ 2, 4, 6 }, { -5, 7, 9 });
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
