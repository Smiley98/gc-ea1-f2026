#include "raylib.h"
#include "raymath.h"
#include "raygui.h"

#include <array>
#include <vector>
#include <algorithm>

constexpr float BALL_RADIUS = 25.0f;
constexpr Vector2 GRAVITY = { 0.0f, 100.0f };

struct Entity
{
    Vector2 pos;
    Vector2 vel;
    Vector2 acc;
    float gravity_scale;
    Color color;

    bool destroy;
};

void Update(Entity& entity, float dt)
{
    entity.acc = GRAVITY * entity.gravity_scale;
    entity.vel += entity.acc * dt;
    entity.pos += entity.vel * dt;
}

int main()
{
    InitWindow(800, 800, "Game");
    InitAudioDevice();
    SetTargetFPS(60);

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

    std::vector<Entity> entities;
    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        if (IsKeyPressed(KEY_SPACE))
        {
            Entity ball{};
            ball.color = BLUE;
            ball.pos = ball_launch_position;
            ball.vel = Vector2Rotate(Vector2UnitX, -30.0f * DEG2RAD) * 200.0f;
            ball.gravity_scale = 1.0f;
            entities.push_back(ball);
        }

        for (Entity& e : entities)
            Update(e, GetFrameTime());
        
        for (Entity& e : entities)
            e.destroy = e.pos.y + BALL_RADIUS >= ground.y;

        std::erase_if(entities, [](Entity e) { return e.destroy; });

        BeginDrawing();
            ClearBackground(WHITE);
            DrawRectangleRec(ground, BEIGE);
            DrawRectangleRec(platform, GRAY);
            for (const Entity& e : entities)
                DrawCircleV(e.pos, BALL_RADIUS, e.color);

            DrawCircleV(GetMousePosition(), 20.0f, RED);
        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
}
