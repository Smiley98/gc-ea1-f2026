#include "ea1.cpp"

enum AppState
{
    APP_STATE_COLLIDER_TEST,
    APP_STATE_BALL_PIT
};

struct BallPit
{
    std::vector<Entity> balls;
};

struct ColliderTest
{

};

void AppUpdateBallPit(BallPit* ball_pit);
void AppUpdateColliderTest(ColliderTest* collider_test);

void AppDrawBallPit(const BallPit& ball_pit);
void AppDrawColliderTest(const ColliderTest& collider_test);

int main()
{
    int state = APP_STATE_BALL_PIT;

    BallPit ball_pit{};
    ball_pit.balls.reserve(128);

    ColliderTest collider_test{};

    InitWindow(800, 800, "Game");
    InitAudioDevice();
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_TAB))
        {
            ++state %= 2;
            if (state == APP_STATE_BALL_PIT)
                ball_pit.balls.clear();
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

void AppUpdateColliderTest(ColliderTest* collider_test)
{
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

void AppDrawColliderTest(const ColliderTest& collider_test)
{
    ClearBackground(WHITE);
}
