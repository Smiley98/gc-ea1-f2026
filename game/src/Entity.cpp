
void EntityUpdate(Entity& entity, float dt)
{
    Vector2 acc = entity.force * entity.mass_inverse;
    acc += GRAVITY * entity.gravity_scale;
    entity.vel += acc * dt;
    entity.pos += entity.vel * dt;
    entity.force = Vector2Zeros;
}
