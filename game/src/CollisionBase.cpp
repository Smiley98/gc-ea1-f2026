
///////////////////////////////
// Primitive functions begin //
///////////////////////////////

bool HitTestCircles(Vector2 pos_a, float radius_a, Vector2 pos_b, float radius_b, Vector2* mtv)
{
    // Follow slide 28
    return false;
}

bool HitTestCircleCapsule(Vector2 pos_circle, float radius_circle, Vector2 pos_capsule, float radius_capsule, float half_length, Vector2 direction, Vector2* mtv)
{
    return false;
}

bool HitTestCircleAABB(Vector2 pos_circle, float radius, Vector2 pos_aabb, Vector2 half_extents, Vector2* mtv)
{
    return false;
}

bool HitTestCapsuleAABB(Vector2 pos_capsule, float radius_capsule, float half_height, Vector2 direction, Vector2 pos_aabb, Vector2 half_extents, Vector2* mtv)
{
    return false;
}

bool HitTestAABBs(Vector2 pos_a, Vector2 half_extents_a, Vector2 pos_b, Vector2 half_extents_b, Vector2* mtv)
{
    return false;
}

bool HitTestCapsules(
    Vector2 pos_a, float radius_a, float half_length_a, Vector2 direction_a,
    Vector2 pos_b, float radius_b, float half_length_b, Vector2 direction_b, Vector2* mtv)
{
    return false;
}

///////////////////////////////
// Primitive functions end ////
///////////////////////////////