
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
