
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
    bool result = HitTestCircles(pos_a, col_a.circle.radius, pos_b, col_b.circle.radius, mtv);
    return result;
}

bool HitTestCircleAABB(Vector2 pos_a, Collider col_a, Vector2 pos_b, Collider col_b, Vector2* mtv)
{
    bool result = HitTestCircleAABB(pos_a, col_a.circle.radius, pos_b, col_b.aabb.half_extents, mtv);
    return result;
}

bool HitTestAABBCircle(Vector2 pos_a, Collider col_a, Vector2 pos_b, Collider col_b, Vector2* mtv)
{
    bool result = HitTestCircleAABB(pos_b, col_b.circle.radius, pos_a, col_a.aabb.half_extents, mtv);
    return result;
}

bool HitTestAABBs(Vector2 pos_a, Collider col_a, Vector2 pos_b, Collider col_b, Vector2* mtv)
{
    bool result = HitTestAABBs(pos_a, col_a.aabb.half_extents, pos_b, col_b.aabb.half_extents, mtv);
    return result;
}

bool HitTestCircleCapsule(Vector2 pos_a, Collider col_a, Vector2 pos_b, Collider col_b, Vector2* mtv)
{
    // Follow slide 33
    bool result = HitTestCircleCapsule(pos_a, col_a.circle.radius, pos_b, col_b.capsule.radius, col_b.capsule.half_length, col_b.capsule.direction, mtv);
    return result;
}

bool HitTestCapsuleCircle(Vector2 pos_a, Collider col_a, Vector2 pos_b, Collider col_b, Vector2* mtv)
{
    // The above but parameters are reversed
    bool result = HitTestCircleCapsule(pos_b, col_b.circle.radius, pos_a, col_a.capsule.radius, col_a.capsule.half_length, col_a.capsule.direction, mtv);
    return result;
}

bool HitTestCapsules(Vector2 pos_a, Collider col_a, Vector2 pos_b, Collider col_b, Vector2* mtv)
{
    // Follow slide 50
    bool result = HitTestCapsules(
        pos_a, col_a.capsule.radius, col_a.capsule.half_length, col_a.capsule.direction,
        pos_b, col_b.capsule.radius, col_b.capsule.half_length, col_b.capsule.direction, mtv);
    return result;
}

bool HitTestAABBCapsule(Vector2 pos_a, Collider col_a, Vector2 pos_b, Collider col_b, Vector2* mtv)
{
    // Follow slide 51
    bool result = HitTestCapsuleAABB(pos_b, col_b.capsule.radius, col_b.capsule.half_length, col_b.capsule.direction, pos_a, col_a.aabb.half_extents, mtv);
    return result;
}

bool HitTestCapsuleAABB(Vector2 pos_a, Collider col_a, Vector2 pos_b, Collider col_b, Vector2* mtv)
{
    // The above but parameters are reversed
    bool result = HitTestCapsuleAABB(pos_a, col_a.capsule.radius, col_a.capsule.half_length, col_a.capsule.direction, pos_b, col_b.aabb.half_extents, mtv);
    return result;
}
//////////////////////////////
// Collider overloads end ////
//////////////////////////////