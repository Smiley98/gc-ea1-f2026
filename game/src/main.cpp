#include "raylib.h"
#include "raymath.h"
#include "raygui.h"

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

bool InFoV(Vector2 viewerPosition, Vector2 viewerDirection, float viewDistance, float FoV/*radians*/, Vector2 targetPosition)
{
    // Don't bother with angle-check if target is far away from viewer
    if (Vector2Distance(viewerPosition, targetPosition) > viewDistance) return false;
    // Optimization: compare squared distances to avoid expensive square-root calculations
    //if (DistanceSqr(viewerPosition, targetPosition) > viewDistance * viewDistance) return false;

    // Vector from A to B = B - A
    Vector2 targetDirection = Vector2Normalize(targetPosition - viewerPosition);

    // Determine if the angle between viewer and target (dot product) is less than
    // half the viewer's field of view (half to form a right-triangle)
    float angle = acosf(Vector2DotProduct(viewerDirection, targetDirection));
    return angle <= FoV * 0.5f;

    // Optimization: Since arccos is the inverse of cos, we can flip the comparison to avoid an inverse trig function
    //return Dot(viewerDirection, targetDirection) > cosf(FoV * 0.5f);
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game");
    SetTargetFPS(60);

    float fov = 60.0f;  // field of view
    float viewDistance = 500.0f;
    float rotation = 0.0f;
    Vector2 viewerPosition{ SCREEN_WIDTH * 0.25f, SCREEN_HEIGHT * 0.5f };
    Vector2 targetPosition{ SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f };

    while (!WindowShouldClose())
    {
        Vector2 viewerDirection = Vector2Rotate(Vector2UnitX, rotation * DEG2RAD);
        bool targetVisible = InFoV(viewerPosition, viewerDirection, viewDistance, fov * DEG2RAD, targetPosition);

        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawCircleV(viewerPosition, 25.0f, BLUE);
            DrawLineV(viewerPosition, viewerPosition + Vector2Rotate(viewerDirection, fov * DEG2RAD * 0.5f) * viewDistance, BLUE);
            DrawLineV(viewerPosition, viewerPosition + Vector2Rotate(viewerDirection, fov * DEG2RAD * -0.5f) * viewDistance, BLUE);
            DrawCircleV(targetPosition, 25.0f, targetVisible ? GREEN : RED);

            GuiSlider({ 20, 10, 160, 80 }, "50", "500", &viewDistance, 50.0f, 500.0f);
            GuiSlider({ 20, 100, 160, 80 }, "10", "150", &fov, 10.0f, 150.0f);
            GuiSlider({ 20, 190, 160, 80 }, "0", "360", &rotation, 0.0f, 360.0f);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}