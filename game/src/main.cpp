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

enum ColliderType
{
    COLLIDER_TYPE_NONE,
    COLLIDER_TYPE_CIRCLE,
    COLLIDER_TYPE_AABB,
    COLLIDER_TYPE_CAPSULE
};

struct CircleCollider
{
    float radius;
};

struct AABBCollider
{
    Vector2 half_extents;
};

struct CapsuleCollider
{
    float radius;
    Vector2 half_extents;
    Vector2 direction;
};

struct Collider
{
    ColliderType type;
    union
    {
        CircleCollider circle;
        AABBCollider aabb;
        CapsuleCollider capsule;
    };
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

    Collider collider;
};

void Update(Entity& entity, float dt)
{
    entity.acc = GRAVITY * entity.gravity_scale;
    entity.vel += entity.acc * dt;
    entity.pos += entity.vel * dt;
}

Rectangle RecFromAABB(Vector2 pos, Vector2 half_extents)
{
    Rectangle rec{};
    rec.x = pos.x - half_extents.x;
    rec.y = pos.y - half_extents.y;
    rec.width = half_extents.x * 2.0f;
    rec.height = half_extents.y * 2.0f;
    return rec;
}

bool HitTestNone(Vector2 pos_a, Collider col_a, Vector2 pos_b, Collider col_b)
{
    assert(false);
    return false;
}

bool HitTestCircles(Vector2 pos_a, Collider col_a, Vector2 pos_b, Collider col_b)
{
    bool result = CheckCollisionCircles(pos_a, col_a.circle.radius, pos_b, col_b.circle.radius);
    return result;
}

bool HitTestCircleAABB(Vector2 pos_a, Collider col_a, Vector2 pos_b, Collider col_b)
{
    bool result = CheckCollisionCircleRec(pos_a, col_a.circle.radius, RecFromAABB(pos_b, col_b.aabb.half_extents));
    return result;
}

bool HitTestAABBCircle(Vector2 pos_a, Collider col_a, Vector2 pos_b, Collider col_b)
{
    bool result = CheckCollisionCircleRec(pos_b, col_b.circle.radius, RecFromAABB(pos_a, col_a.aabb.half_extents));
    return result;
}

bool HitTestAABBs(Vector2 pos_a, Collider col_a, Vector2 pos_b, Collider col_b)
{
    bool result = CheckCollisionRecs(RecFromAABB(pos_a, col_a.aabb.half_extents), RecFromAABB(pos_b, col_b.aabb.half_extents));
    return result;
}

using CollisionFunc = bool(*)(Vector2 pos_a, Collider col_a, Vector2 pos_b, Collider col_b);

CollisionFunc COLLISION_TABLE[3][3] =
{
    // NONE         // CIRCLE    // AABB
    HitTestNone, HitTestNone,       HitTestNone,        // NONE
    HitTestNone, HitTestCircles,    HitTestCircleAABB,  // CIRCLE
    HitTestNone, HitTestAABBCircle, HitTestAABBs,       // AABBs
};

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

        target.collider.type = i % 2 == 0 ? COLLIDER_TYPE_CIRCLE : COLLIDER_TYPE_AABB;
        target.collider.aabb.half_extents = Vector2Ones * BALL_RADIUS;

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
            ball.collider.type = COLLIDER_TYPE_CIRCLE;
            ball.collider.circle.radius = BALL_RADIUS;
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
                CollisionFunc func = COLLISION_TABLE[a.collider.type][b.collider.type];
                bool collision = func(a.pos, a.collider, b.pos, b.collider);
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
            {
                switch (e.collider.type)
                {
                case COLLIDER_TYPE_CIRCLE:
                    DrawCircleV(e.pos, e.collider.circle.radius, e.color);
                    break;

                case COLLIDER_TYPE_AABB:
                    DrawRectangleRec(RecFromAABB(e.pos, e.collider.aabb.half_extents), e.color);
                    break;
                }
            }
                
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
