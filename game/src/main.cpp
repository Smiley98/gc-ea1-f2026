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
    float half_length;
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

CapsuleCollider CapsuleFromPoints(Vector2 bottom, Vector2 top, float radius, Vector2* position = nullptr)
{
    CapsuleCollider capsule{};

    capsule.radius = radius;
    capsule.half_length = (Vector2Distance(bottom, top) - radius * 2.0f) * 0.5f;
    capsule.direction = Vector2Normalize(top - bottom);

    if (position != nullptr)
        *position = (bottom + top) * 0.5f;

    return capsule;
}

RMAPI Vector2 Vector2ProjectPointLine(Vector2 P, Vector2 A, Vector2 B)
{
    Vector2 AB = (B - A);
    float t = Vector2DotProduct((P - A), AB) / Vector2DotProduct(AB, AB);
    return A + (AB * Clamp(t, 0.0f, 1.0f));
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

bool HitTestCircleCapsule(Vector2 pos_a, Collider col_a, Vector2 pos_b, Collider col_b)
{
    // Follow slide 33
    bool result = false;
    return result;
}

bool HitTestCapsuleCircle(Vector2 pos_a, Collider col_a, Vector2 pos_b, Collider col_b)
{
    // The above but parameters are reversed
    bool result = false;
    return result;
}

bool HitTestCapsules(Vector2 pos_a, Collider col_a, Vector2 pos_b, Collider col_b)
{
    // Follow slide 33
    bool result = false;
    return result;
}

bool HitTestAABBCapsule(Vector2 pos_a, Collider col_a, Vector2 pos_b, Collider col_b)
{
    // The above but parameters are reversed
    bool result = false;
    return result;
}

bool HitTestCapsuleAABB(Vector2 pos_a, Collider col_a, Vector2 pos_b, Collider col_b)
{
    // The above but parameters are reversed
    bool result = false;
    return result;
}

using CollisionFunc = bool(*)(Vector2 pos_a, Collider col_a, Vector2 pos_b, Collider col_b);

CollisionFunc COLLISION_TABLE[4][4] =
{
    // NONE      // CIRCLE              // AABB             //CAPSULE
    HitTestNone, HitTestNone,           HitTestNone,        HitTestNone,            // NONE
    HitTestNone, HitTestCircles,        HitTestCircleAABB,  HitTestCircleCapsule,   // CIRCLE
    HitTestNone, HitTestAABBCircle,     HitTestAABBs,       HitTestNone,            // AABBs
    HitTestNone, HitTestCapsuleCircle,  HitTestNone,        HitTestNone             // CAPSULE
};

void DrawCapsuleCollider(Vector2 position, CapsuleCollider collider, Color color)
{
    Rectangle rec;
    rec.x = position.x;
    rec.y = position.y;
    rec.width = collider.half_length * 2.0f;
    rec.height = collider.radius * 2.0f;

    float rotation = Vector2Angle(Vector2UnitX, collider.direction) * RAD2DEG;

    Vector2 top = position + collider.direction * collider.half_length;
    Vector2 bot = position - collider.direction * collider.half_length;

    DrawRectanglePro(rec, { collider.half_length, collider.radius }, rotation, color);
    DrawCircleV(top, collider.radius, color);
    DrawCircleV(bot, collider.radius, color);

    DrawLineEx(position, top, collider.half_length * 0.1f, BLUE);
    DrawLineEx(top, top + Vector2Rotate(collider.direction, 45.0f * DEG2RAD) * collider.radius, collider.radius * 0.1f, SKYBLUE);

    DrawCircleV(top, collider.radius * 0.25f, DARKBLUE);
    DrawCircleV(bot, collider.radius * 0.25f, MAGENTA);
}

void DrawCollider(Vector2 position, Collider collider, Color color)
{
    switch (collider.type)
    {
    case COLLIDER_TYPE_CIRCLE:
        DrawCircleV(position, collider.circle.radius, color);
        break;

    case COLLIDER_TYPE_AABB:
        DrawRectangleRec(RecFromAABB(position, collider.aabb.half_extents), color);
        break;

    case COLLIDER_TYPE_CAPSULE:
        DrawCapsuleCollider(position, collider.capsule, color);
        break;
    }
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

            //DrawRectangleRec(ground, BEIGE);
            //DrawRectangleRec(platform, GRAY);
            //
            //for (const Entity& e : entities)
            //    DrawCollider(e.pos, e.collider, e.color);
            //    
            //DrawLineEx(ball_launch_position, ball_launch_position + ball_launch_velocity, 4.0f, ORANGE);

            // Representation 1
            {
                CapsuleCollider cap;
                cap.radius = 20.0f;
                cap.half_length = 40.0f;
                cap.direction = Vector2UnitX;
                DrawCapsuleCollider({ 440.0f, 720.0f }, cap, PINK);
            }

            // Representation 2
            {
                float r = 20.0f;
                float hl = 40.0f;
                float dist = (r + hl) * 2.0f;

                Vector2 bottom = { 380.0f, 660.0f };
                Vector2 top = bottom + Vector2UnitX * dist;

                Vector2 position;
                CapsuleCollider cap = CapsuleFromPoints(bottom, top, r, &position);
            
                DrawCapsuleCollider(position, cap, PURPLE);
            }

            Vector2 rec_pos = { 400.0f, 400.0f };
            Vector2 half_extents = { 80.0f, 40.0f };
            DrawRectangleRec(RecFromAABB(rec_pos, half_extents), PURPLE);

            Vector2 p = GetMousePosition();
            Vector2 proj = Vector2Clamp(p, rec_pos - half_extents, rec_pos + half_extents);
            DrawCircleV(p, 20.0f, BLUE);
            DrawCircleV(proj, 20.0f, ORANGE);

            //Vector2 a = { 200.0f, 200.0f };
            //Vector2 b = { 600.0f, 600.0f };
            //Vector2 p = GetMousePosition();
            //
            //DrawLineEx(a, b, 4.0f, BLUE);
            //DrawCircleV(p, 20.0f, PURPLE);
            //Vector2 c = Vector2ProjectPointLine(p, a, b);
            //DrawCircleV(c, 20.0f, ORANGE);

            DrawCircleV(GetMousePosition(), 20.0f, RED);
        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
}
