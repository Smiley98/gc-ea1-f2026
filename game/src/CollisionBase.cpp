
///////////////////////////////
// Primitive functions begin //
///////////////////////////////

// MTV resolves A from B (points from B to A)
bool HitTestCircles(Vector2 pos_a, float radius_a, Vector2 pos_b, float radius_b, Vector2* mtv)
{
    float radii_sum = radius_a + radius_b;
    bool collision = Vector2DistanceSqr(pos_a, pos_b) <= radii_sum * radii_sum;
    if (collision && mtv != nullptr)
    {
        *mtv = Vector2Normalize(pos_a - pos_b) * (radii_sum - Vector2Distance(pos_a, pos_b));
    }
    return collision;
}

// MTV resolves A from B (points from B to A)
bool HitTestAABBs(Vector2 pos_a, Vector2 half_extents_a, Vector2 pos_b, Vector2 half_extents_b, Vector2* mtv)
{
    // TODO -- complete this xD
    return false;
}

// MTV resolves A from B (points from B to A)
bool HitTestCapsules(
    Vector2 pos_a, float radius_a, float half_length_a, Vector2 direction_a,
    Vector2 pos_b, float radius_b, float half_length_b, Vector2 direction_b, Vector2* mtv)
{
    Vector2 top1 = pos_a + direction_a * half_length_a;
    Vector2 bot1 = pos_a - direction_a * half_length_a;
    Vector2 top2 = pos_b + direction_b * half_length_b;
    Vector2 bot2 = pos_b - direction_b * half_length_b;
    Vector2 proj1, proj2;
    NearestPointsOnLines(bot1, top1, bot2, top2, &proj1, &proj2);
    bool collision = HitTestCircles(proj1, radius_a, proj2, radius_b, mtv);
    return collision;
}

// MTV resolves A from B (points from B to A)
bool HitTestCircleCapsule(Vector2 pos_circle, float radius_circle, Vector2 pos_capsule, float radius_capsule, float half_length, Vector2 direction, Vector2* mtv)
{
    Vector2 top = pos_capsule + direction * half_length;
    Vector2 bot = pos_capsule - direction * half_length;
    Vector2 proj = Vector2ProjectPointLine(pos_circle, bot, top);
    bool collision = HitTestCircles(pos_capsule, radius_circle, proj, radius_capsule, mtv);
    return collision;
}

// MTV resolves A from B (points from B to A)
bool HitTestCircleAABB(Vector2 pos_circle, float radius, Vector2 pos_aabb, Vector2 half_extents, Vector2* mtv)
{
    Vector2 proj = Vector2Clamp(pos_circle, pos_aabb - half_extents, pos_aabb + half_extents);
    bool collision = HitTestCircles(pos_circle, radius, proj, 0.0f, mtv);
    return collision;
}

// MTV resolves A from B (points from B to A)
bool HitTestCapsuleAABB(Vector2 pos_capsule, float radius_capsule, float half_length, Vector2 direction, Vector2 pos_aabb, Vector2 half_extents, Vector2* mtv)
{
    Vector2 proj = Vector2ProjectPointLine(pos_aabb, pos_capsule + half_length * direction, pos_capsule - half_length * direction);
    Vector2 clamp = Vector2Clamp(proj, pos_aabb - half_extents, pos_aabb + half_extents);
    bool collision = HitTestCircles(proj, radius_capsule, clamp, 0.0f, mtv);
    return collision;
}

///////////////////////////////
// Primitive functions end ////
///////////////////////////////
