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
            EntityUpdate(e, GetFrameTime());
        }
        
        for (Entity& e : entities)
            e.destroy = e.pos.y + BALL_RADIUS >= ground.y;

        for (size_t i = 0; i < entities.size(); i++)
        {
            for (size_t j = i + 1; j < entities.size(); j++)
            {
                Entity& a = entities[i];
                Entity& b = entities[j];

                Vector2 mtv = Vector2Zeros;
                CollisionFunc func = COLLISION_TABLE[a.collider.type][b.collider.type];
                bool collision = func(a.pos, a.collider, b.pos, b.collider, &mtv);
                bool same_type = a.type == b.type;
                bool destroy = collision && !same_type;
                a.destroy |= destroy;
                b.destroy |= destroy;
            }
        }

        std::erase_if(entities, [](Entity e) { return e.destroy; });

        BeginDrawing();
            ClearBackground(WHITE);

            // Point-rectangle projection illustration
            {
                Vector2 rec_pos = { 400.0f, 400.0f };
                Vector2 half_extents = { 80.0f, 40.0f };
                DrawRectangleRec(RecFromAABB(rec_pos, half_extents), PURPLE);
            
                Vector2 p = GetMousePosition();
                Vector2 proj = Vector2Clamp(p, rec_pos - half_extents, rec_pos + half_extents);
                DrawCircleV(p, 20.0f, BLUE);
                DrawCircleV(proj, 20.0f, ORANGE);
            }
            
            // Point-line projection illustration
            //{
            //    Vector2 a = { 200.0f, 200.0f };
            //    Vector2 b = { 600.0f, 600.0f };
            //    Vector2 p = GetMousePosition();
            //
            //    DrawLineEx(a, b, 4.0f, BLUE);
            //    DrawCircleV(p, 20.0f, PURPLE);
            //    Vector2 c = Vector2ProjectPointLine(p, a, b);
            //    DrawCircleV(c, 20.0f, ORANGE);
            //}

        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
}
