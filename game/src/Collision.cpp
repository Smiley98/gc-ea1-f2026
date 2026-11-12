
// MTV points FROM B TO A (resolves A from B)
using CollisionFunc = bool(*)(Vector2 pos_a, Collider col_a, Vector2 pos_b, Collider col_b, Vector2* mtv);

//////////////////////////////
// Collider overloads begin //
//////////////////////////////

bool HitTestNone(Vector2 pos_a, Collider col_a, Vector2 pos_b, Collider col_b, Vector2* mtv)
{
    assert(false);
    return false;
}

bool HitTestCircles(Vector2 pos_a, Collider col_a, Vector2 pos_b, Collider col_b, Vector2* mtv)
{
    bool collision = HitTestCircles(pos_a, col_a.circle.radius, pos_b, col_b.circle.radius, mtv);
    return collision;
}

bool HitTestCircleAABB(Vector2 pos_a, Collider col_a, Vector2 pos_b, Collider col_b, Vector2* mtv)
{
    bool collision = HitTestCircleAABB(pos_a, col_a.circle.radius, pos_b, col_b.aabb.half_extents, mtv);
    return collision;
}

bool HitTestAABBCircle(Vector2 pos_a, Collider col_a, Vector2 pos_b, Collider col_b, Vector2* mtv)
{
    bool collision = HitTestCircleAABB(pos_b, col_b.circle.radius, pos_a, col_a.aabb.half_extents, mtv);
    if (collision && mtv != nullptr)
        *mtv *= -1.0f;
    return collision;
}

bool HitTestAABBs(Vector2 pos_a, Collider col_a, Vector2 pos_b, Collider col_b, Vector2* mtv)
{
    bool collision = HitTestAABBs(pos_a, col_a.aabb.half_extents, pos_b, col_b.aabb.half_extents, mtv);
    return collision;
}

bool HitTestCircleCapsule(Vector2 pos_a, Collider col_a, Vector2 pos_b, Collider col_b, Vector2* mtv)
{
    bool collision = HitTestCircleCapsule(pos_a, col_a.circle.radius, pos_b, col_b.capsule.radius, col_b.capsule.half_length, col_b.capsule.direction, mtv);
    return collision;
}

bool HitTestCapsuleCircle(Vector2 pos_a, Collider col_a, Vector2 pos_b, Collider col_b, Vector2* mtv)
{
    bool collision = HitTestCircleCapsule(pos_b, col_b.circle.radius, pos_a, col_a.capsule.radius, col_a.capsule.half_length, col_a.capsule.direction, mtv);
    if (collision && mtv != nullptr)
        *mtv *= -1.0f;
    return collision;
}

bool HitTestCapsules(Vector2 pos_a, Collider col_a, Vector2 pos_b, Collider col_b, Vector2* mtv)
{
    bool collision = HitTestCapsules(
        pos_a, col_a.capsule.radius, col_a.capsule.half_length, col_a.capsule.direction,
        pos_b, col_b.capsule.radius, col_b.capsule.half_length, col_b.capsule.direction, mtv);
    return collision;
}

bool HitTestAABBCapsule(Vector2 pos_a, Collider col_a, Vector2 pos_b, Collider col_b, Vector2* mtv)
{
    bool collision = HitTestCapsuleAABB(pos_b, col_b.capsule.radius, col_b.capsule.half_length, col_b.capsule.direction, pos_a, col_a.aabb.half_extents, mtv);
    if (collision && mtv != nullptr)
        *mtv *= -1.0f;
    return collision;
}

bool HitTestCapsuleAABB(Vector2 pos_a, Collider col_a, Vector2 pos_b, Collider col_b, Vector2* mtv)
{
    bool collision = HitTestCapsuleAABB(pos_a, col_a.capsule.radius, col_a.capsule.half_length, col_a.capsule.direction, pos_b, col_b.aabb.half_extents, mtv);
    return collision;
}

//////////////////////////////
// Collider overloads end ////
//////////////////////////////

CollisionFunc COLLISION_TABLE[4][4] =
{
    // NONE      // CIRCLE              // AABB             //CAPSULE
    HitTestNone, HitTestNone,           HitTestNone,        HitTestNone,            // NONE
    HitTestNone, HitTestCircles,        HitTestCircleAABB,  HitTestCircleCapsule,   // CIRCLE
    HitTestNone, HitTestAABBCircle,     HitTestAABBs,       HitTestAABBCapsule,     // AABBs
    HitTestNone, HitTestCapsuleCircle,  HitTestCapsuleAABB, HitTestCapsules         // CAPSULE
};
