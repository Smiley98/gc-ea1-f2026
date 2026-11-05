#include <raylib.h>
#include <raymath.h>
#include <raygui.h>

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

#include "Math.cpp"
#include "Geometry.cpp"
#include "CollisionBase.cpp"
#include "Collision.cpp"
#include "Entity.cpp"
