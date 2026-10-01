#include "raylib.h"
#include "raymath.h"
#include "raygui.h"

#include <cassert>
#include <array>
#include <vector>
#include <algorithm>

constexpr float BALL_RADIUS = 25.0f;
constexpr Vector2 GRAVITY = { 0.0f, 100.0f };

enum EntityType
{
    ENTITY_TYPE_NONE,
    ENTITY_TYPE_BALL,
    ENTITY_TYPE_TARGET
};

struct Entity
{
    Vector2 pos;
    Vector2 vel;
    Vector2 acc;
    float gravity_scale;
    Color color;

    EntityType type;
    bool destroy;
};

void Update(Entity& entity, float dt)
{
    entity.acc = GRAVITY * entity.gravity_scale;
    entity.vel += entity.acc * dt;
    entity.pos += entity.vel * dt;
}

struct Data
{
    // Union shares memory regions
    // Size is equal to the largest memory layout (values == 12 bytes, ab == 8 bytes, so Data == 12 bytes)
    union
    {
        struct
        {
            int a;
            int b;
        };
        struct
        {
            int values[3];
        };
    };
};

int main()
{
    Data d{};
    d.a = 1;
    d.values[1] = 2;
    int sz = sizeof(d);

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
    float ball_launch_angle = 30.0f;
    ball_launch_position.x = platform.x + BALL_RADIUS;
    ball_launch_position.y = platform.y - BALL_RADIUS;

    std::vector<Entity> entities;
    entities.resize(10);

    for (size_t i = 0; i < entities.size(); i++)
    {
        Entity& target = entities[i];
        target.type = ENTITY_TYPE_TARGET;
        target.pos.x = 150.0f + i * 50.0f;
        target.pos.y = (50.0f + 50 * (i < 5 ? 10 - i : i));

        //launch_position.y - (100.0f + 50 * (i > 5 ? 10 - i : i));
        target.color = RED;
    }

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        if (IsKeyDown(KEY_UP))
        {
            ball_launch_angle += 100.0f * dt;
        }
        if (IsKeyDown(KEY_DOWN))
        {
            ball_launch_angle -= 100.0f * dt;
        }

        Vector2 ball_launch_velocity = Vector2Rotate(Vector2UnitX, -ball_launch_angle * DEG2RAD) * 300.0f;

        if (IsKeyPressed(KEY_SPACE))
        {
            Entity ball{};
            ball.type = ENTITY_TYPE_BALL;
            ball.color = BLUE;
            ball.pos = ball_launch_position;
            ball.vel = ball_launch_velocity;
            ball.gravity_scale = 1.0f;
            entities.push_back(ball);
        }

        for (Entity& e : entities)
        {
            assert(e.type != ENTITY_TYPE_NONE);
            Update(e, GetFrameTime());
        }
        
        for (Entity& e : entities)
            e.destroy = e.pos.y + BALL_RADIUS >= ground.y;

        for (size_t i = 0; i < entities.size(); i++)
        {
            for (size_t j = i + 1; j < entities.size(); j++)
            {
                Entity& a = entities[i];
                Entity& b = entities[j];
                bool collision = CheckCollisionCircles(a.pos, BALL_RADIUS, b.pos, BALL_RADIUS);
                bool same_type = a.type == b.type;
                bool destroy = collision && !same_type;
                a.destroy |= destroy;
                b.destroy |= destroy;
            }
        }

        std::erase_if(entities, [](Entity e) { return e.destroy; });

        BeginDrawing();
            ClearBackground(WHITE);
            DrawRectangleRec(ground, BEIGE);
            DrawRectangleRec(platform, GRAY);

            for (const Entity& e : entities)
                DrawCircleV(e.pos, BALL_RADIUS, e.color);

            DrawLineEx(ball_launch_position, ball_launch_position + ball_launch_velocity, 4.0f, ORANGE);

            // % codes for data-types https://cplusplus.com/reference/cstdio/printf/
            // TLDR %i for int, %f for float, %s for string
            float time = GetTime();
            const char* time_text = TextFormat("Time: %f", time);
            DrawText(TextFormat("Time: %f", time), 10, 10, 20, DARKGRAY);

            DrawCircleV(GetMousePosition(), 20.0f, RED);
        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
}
