#include "ea1.cpp"

enum AppState
{
    APP_STATE_BALL_PIT,
    APP_STATE_COLLIDER_TEST,
    APP_STATE_FORCES
};

struct BallPit
{
    std::vector<Entity> balls;
};

struct ColliderTest
{
    std::array<Collider, 4> templates;

    Entity a;
    Entity b;
};

struct Forces
{
    Entity light;
    Entity medium;
    Entity heavy;
};

void AppInitBallPit(BallPit* ball_pit);
void AppInitColliderTest(ColliderTest* collider_test);
void AppInitForces(Forces* forces);

void AppUpdateColliderTest(ColliderTest* collider_test);
void AppUpdateBallPit(BallPit* ball_pit);
void AppUpdateForces(Forces* forces);

void AppDrawColliderTest(ColliderTest& collider_test);
void AppDrawBallPit(const BallPit& ball_pit);
void AppDrawForces(const Forces& forces);

void AppReset(int state, ColliderTest* collider_test, BallPit* ball_pit, Forces* forces);

int main()
{
    InitWindow(800, 800, "Game");
    InitAudioDevice();
    SetTargetFPS(60);

    int state = APP_STATE_FORCES;
    BallPit ball_pit{};
    ColliderTest collider_test{};
    Forces forces{};
    AppReset(state, &collider_test, &ball_pit, &forces);

    int frame_count = 0;
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_TAB))
        {
            ++state %= 3;
            AppReset(state, &collider_test, &ball_pit, &forces);
        }

        if (IsKeyPressed(KEY_R))
        {
            AppReset(state, &collider_test, &ball_pit, &forces);
        }

        // Don't apply motion until GetFrameTime() is stable
        if (frame_count >= 2)
        {
            switch (state)
            {
            case APP_STATE_BALL_PIT:
                AppUpdateBallPit(&ball_pit);
                break;
            case APP_STATE_COLLIDER_TEST:
                AppUpdateColliderTest(&collider_test);
                break;
            case APP_STATE_FORCES:
                AppUpdateForces(&forces);
                break;
            }
        }
        
        BeginDrawing();
        switch (state)
        {
        case APP_STATE_BALL_PIT:
            AppDrawBallPit(ball_pit);
            break;
        case APP_STATE_COLLIDER_TEST:
            AppDrawColliderTest(collider_test);
            break;
        case APP_STATE_FORCES:
            AppDrawForces(forces);
            break;
        }
        EndDrawing();

        frame_count++;
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
}

void AppUpdateBallPit(BallPit* ball_pit)
{
    std::vector<Entity>& balls = ball_pit->balls;

    float dt = GetFrameTime();
    static float spawn_timer = 0.0f;
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

    // Optimization to avoid copying 127 elements: swap last with first, then remove last!
    if (balls.size() >= 128)
    {
        balls.front() = balls.back();
        balls.pop_back();
    }

    for (size_t i = 0; i < balls.size(); i++)
    {
        for (size_t j = i + 1; j < balls.size(); j++)
        {
            Entity& a = balls[i];
            Entity& b = balls[j];

            Vector2 mtv = Vector2Zeros;
            CollisionFunc func = COLLISION_TABLE[a.collider.type][b.collider.type];
            bool collision = func(a.pos, a.collider, b.pos, b.collider, &mtv);

            if (collision)
            {
                a.pos += mtv * 0.5f;
                b.pos -= mtv * 0.5f;

                a.vel *= -1.0f;
                b.vel *= -1.0f;
            }
        }
    }
}

void AppInitBallPit(BallPit* ball_pit)
{
    ball_pit->balls.reserve(128);
    ball_pit->balls.clear();
}

void AppInitColliderTest(ColliderTest* collider_test)
{
    collider_test->templates[COLLIDER_TYPE_CIRCLE].circle.radius = 20.0f;
    collider_test->templates[COLLIDER_TYPE_AABB].aabb.half_extents = { 40.0f, 20.0f };
    collider_test->templates[COLLIDER_TYPE_CAPSULE].capsule.radius = 10.0f;
    collider_test->templates[COLLIDER_TYPE_CAPSULE].capsule.half_length = 80.0f;
    collider_test->templates[COLLIDER_TYPE_CAPSULE].capsule.direction = Vector2Rotate(Vector2UnitX, -30.0f * DEG2RAD);

    collider_test->templates[COLLIDER_TYPE_CIRCLE].type = COLLIDER_TYPE_CIRCLE;
    collider_test->templates[COLLIDER_TYPE_AABB].type = COLLIDER_TYPE_AABB;
    collider_test->templates[COLLIDER_TYPE_CAPSULE].type = COLLIDER_TYPE_CAPSULE;

    Entity& a = collider_test->a;
    Entity& b = collider_test->b;
    a.collider = collider_test->templates[COLLIDER_TYPE_AABB];
    b.collider = collider_test->templates[COLLIDER_TYPE_CAPSULE];
    a.pos = { GetScreenWidth() * 0.25f, GetScreenHeight() * 0.5f };
    b.pos = { GetScreenWidth() * 0.75f, GetScreenHeight() * 0.5f };
}

void AppInitForces(Forces* forces)
{
    Entity& l = forces->light;
    Entity& m = forces->medium;
    Entity& h = forces->heavy;
    l = {};
    m = {};
    h = {};

    l.pos = { GetScreenWidth() * 0.25f, GetScreenHeight() * 0.5f };
    l.gravity_scale = 1.0f;
    l.mass_inverse = 1.0f / 1.0f;

    m.pos = { GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f };
    m.gravity_scale = 1.0f;
    m.mass_inverse = 1.0f / 2.0f;

    h.pos = { GetScreenWidth() * 0.75f, GetScreenHeight() * 0.5f };
    h.gravity_scale = 1.0f;
    h.mass_inverse = 1.0f / 5.0f;

    Vector2 launch_force = Vector2UnitY * -10000.0f;
    l.force = m.force = h.force = launch_force;
}

void AppUpdateColliderTest(ColliderTest* test)
{
    Entity& a = test->a;
    Entity& b = test->b;
    b.pos = GetMousePosition();

    Vector2 mtv = Vector2Zeros;
    CollisionFunc func = COLLISION_TABLE[a.collider.type][b.collider.type];
    bool collision = func(a.pos, a.collider, b.pos, b.collider, &mtv);

    a.pos += mtv;
    a.color = collision ? RED : GREEN;
    b.color = collision ? RED : GREEN;
}

void AppUpdateForces(Forces* forces)
{
    float dt = GetFrameTime();
    EntityUpdate(forces->light, dt);
    EntityUpdate(forces->medium, dt);
    EntityUpdate(forces->heavy, dt);
}

void AppDrawBallPit(const BallPit& ball_pit)
{
    ClearBackground(WHITE);
    for (const Entity& ball : ball_pit.balls)
        DrawCollider(ball.pos, ball.collider, ball.color);
    if (!ball_pit.balls.empty())
        DrawText(TextFormat("%i", ball_pit.balls.size()), 10, 10, 20, ball_pit.balls.back().color);
    DrawFPS(720, 5);
}

void AppDrawForces(const Forces& forces)
{
    ClearBackground(WHITE);
    DrawCircleV(forces.light.pos, 20.0f, GREEN);
    DrawCircleV(forces.medium.pos, 20.0f, GREEN);
    DrawCircleV(forces.heavy.pos, 20.0f, GREEN);
}

void AppReset(int state, ColliderTest* collider_test, BallPit* ball_pit, Forces* forces)
{
    if (state == APP_STATE_BALL_PIT)
        AppInitBallPit(ball_pit);
    else if (state == APP_STATE_COLLIDER_TEST)
        AppInitColliderTest(collider_test);
    else if (state == APP_STATE_FORCES)
        AppInitForces(forces);
}

void AppDrawColliderTest(ColliderTest& test)
{
    DrawCollider(test.a.pos, test.a.collider, test.a.color);
    DrawCollider(test.b.pos, test.b.collider, test.b.color);

    static int type_a = test.a.collider.type;
    static int type_b = test.b.collider.type;
    ClearBackground(WHITE);
    GuiToggleGroup({ 10, 10, 90, 30 }, "None A;Circle A;Box A;Capsule A", &type_a);
    GuiToggleGroup({ 10, 45, 90, 30 }, "None B;Circle B;Box B;Capsule B", &type_b);
    test.a.collider = test.templates[type_a];
    test.b.collider = test.templates[type_b];
}
