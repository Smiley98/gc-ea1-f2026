#include "ea1.cpp"

enum AppState
{
    APP_STATE_BALL_PIT,
    APP_STATE_COLLIDER_TEST
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

void AppInitBallPit(BallPit* ball_pit);
void AppInitColliderTest(ColliderTest* collider_test);

void AppUpdateColliderTest(ColliderTest* collider_test);
void AppUpdateBallPit(BallPit* ball_pit);

void AppDrawColliderTest(ColliderTest& collider_test);
void AppDrawBallPit(const BallPit& ball_pit);

int main()
{
    InitWindow(800, 800, "Game");
    InitAudioDevice();
    SetTargetFPS(60);

    int state = APP_STATE_COLLIDER_TEST;
    BallPit ball_pit{};
    ColliderTest collider_test{};
    AppInitBallPit(&ball_pit);
    AppInitColliderTest(&collider_test);

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_TAB))
        {
            ++state %= 2;
            if (state == APP_STATE_BALL_PIT)
                AppInitBallPit(&ball_pit);
            else if (state == APP_STATE_COLLIDER_TEST)
                AppInitColliderTest(&collider_test);
        }

        switch (state)
        {
        case APP_STATE_BALL_PIT:
            AppUpdateBallPit(&ball_pit);
            break;
        case APP_STATE_COLLIDER_TEST:
            AppUpdateColliderTest(&collider_test);
            break;
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
        }
        EndDrawing();
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

    if (balls.size() >= 128) balls.erase(balls.begin());

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
    collider_test->templates[COLLIDER_TYPE_CAPSULE].capsule.radius = 20.0f;
    collider_test->templates[COLLIDER_TYPE_CAPSULE].capsule.half_length = 30.0f;
    collider_test->templates[COLLIDER_TYPE_CAPSULE].capsule.direction = Vector2Rotate(Vector2UnitX, -30.0f * DEG2RAD);

    collider_test->templates[COLLIDER_TYPE_CIRCLE].type = COLLIDER_TYPE_CIRCLE;
    collider_test->templates[COLLIDER_TYPE_AABB].type = COLLIDER_TYPE_AABB;
    collider_test->templates[COLLIDER_TYPE_CAPSULE].type = COLLIDER_TYPE_CAPSULE;

    Entity& a = collider_test->a;
    Entity& b = collider_test->b;
    a.collider = collider_test->templates[COLLIDER_TYPE_CIRCLE];
    b.collider = collider_test->templates[COLLIDER_TYPE_CIRCLE];
    a.pos = { GetScreenWidth() * 0.25f, GetScreenHeight() * 0.5f };
    b.pos = { GetScreenWidth() * 0.75f, GetScreenHeight() * 0.5f };
}

void AppUpdateColliderTest(ColliderTest* test)
{
    Entity& a = test->a;
    Entity& b = test->b;
    Vector2 mtv = Vector2Zeros;
    b.pos = GetMousePosition();
    //CollisionFunc func = COLLISION_TABLE[a.collider.type][b.collider.type];
    //bool collision = func(a.pos, a.collider, b.pos, b.collider, &mtv);

    CollisionFunc func = nullptr;
    if (a.collider.type == COLLIDER_TYPE_CIRCLE && b.collider.type == COLLIDER_TYPE_CAPSULE)
        COLLISION_TABLE[a.collider.type][b.collider.type];
    bool collision = func != nullptr ? func(a.pos, a.collider, b.pos, b.collider, &mtv) : false;

    a.pos += mtv;
    a.color = collision ? RED : GREEN;
    b.color = collision ? RED : GREEN;
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

void AppDrawColliderTest(ColliderTest& test)
{
    DrawCollider(test.a.pos, test.a.collider, test.a.color);
    DrawCollider(test.b.pos, test.b.collider, test.b.color);

    static int type_a = COLLIDER_TYPE_CIRCLE;
    static int type_b = COLLIDER_TYPE_CIRCLE;
    ClearBackground(WHITE);
    GuiToggleGroup({ 10, 10, 90, 30 }, "None A;Circle A;Box A;Capsule A", &type_a);
    GuiToggleGroup({ 10, 45, 90, 30 }, "None B;Circle B;Box B;Capsule B", &type_b);
    test.a.collider = test.templates[type_a];
    test.b.collider = test.templates[type_b];
}
