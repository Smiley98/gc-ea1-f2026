#include "ea1.cpp"

int main()
{
    InitWindow(800, 800, "Game");
    InitAudioDevice();
    SetTargetFPS(60);

    CollisionFunc COLLISION_TABLE[4][4] =
    {
        // NONE      // CIRCLE              // AABB             //CAPSULE
        HitTestNone, HitTestNone,           HitTestNone,        HitTestNone,            // NONE
        HitTestNone, HitTestCircles,        HitTestCircleAABB,  HitTestCircleCapsule,   // CIRCLE
        HitTestNone, HitTestAABBCircle,     HitTestAABBs,       HitTestAABBCapsule,     // AABBs
        HitTestNone, HitTestCapsuleCircle,  HitTestCapsuleAABB, HitTestCapsules         // CAPSULE
    };


    
    std::vector<Entity> balls;
    balls.reserve(128);

    float spawn_timer = 0.0f;
    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        spawn_timer += dt;
        if (spawn_timer >= 0.5f)
        {
            spawn_timer = 0.0f;

            Entity ball{};
            Vector2 dir = Vector2Rotate(Vector2UnitX, GetRandomValue(0, 360) * DEG2RAD);
            ball.pos = { GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f };
            ball.pos += dir * 50.0f;
            ball.vel = dir * 150.0f;
            ball.collider.type = COLLIDER_TYPE_CIRCLE;
            ball.collider.circle.radius = 10.0f;
            ball.color.r = GetRandomValue(0, 255);
            ball.color.g = GetRandomValue(0, 255);
            ball.color.b = GetRandomValue(0, 255);
            ball.color.a = 255;
            balls.push_back(ball);
        }

        for (Entity& ball : balls)
        {
            EntityUpdate(ball, dt);

            if (ball.pos.x + ball.collider.circle.radius >= GetScreenWidth() ||
                ball.pos.x - ball.collider.circle.radius <= 0.0f)
                ball.vel.x *= -1.0f;

            if (ball.pos.y + ball.collider.circle.radius >= GetScreenHeight() ||
                ball.pos.y - ball.collider.circle.radius <= 0.0f)
                ball.vel.y *= -1.0f;
        }

        if (balls.size() >= 128) balls.erase(balls.begin());

        BeginDrawing();
            ClearBackground(WHITE);
            for (const Entity& ball : balls)
                DrawCollider(ball.pos, ball.collider, ball.color);
        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
}

// jk we actually need this
//std::vector<Entity> entities;
//for (size_t i = 0; i < entities.size(); i++)
//{
//    for (size_t j = i + 1; j < entities.size(); j++)
//    {
//        Entity& a = entities[i];
//        Entity& b = entities[j];
//
//        Vector2 mtv = Vector2Zeros;
//        CollisionFunc func = COLLISION_TABLE[a.collider.type][b.collider.type];
//        bool collision = func(a.pos, a.collider, b.pos, b.collider, &mtv);
//    }
//}
//std::erase_if(entities, [](Entity e) { return e.destroy; });
