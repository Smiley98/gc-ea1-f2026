
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