
void EntityUpdate(Entity& entity, float dt)
{
    entity.acc = GRAVITY * entity.gravity_scale;
    entity.vel += entity.acc * dt;
    entity.pos += entity.vel * dt;
}
