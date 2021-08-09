module;
#include <cassert>
module Physics.Ball;

void PhysicsBall::static_collision_response(PhysicsBall* a, PhysicsBall* b)
{
    // Displace the balls apart from each other.
    auto collision_vector = a->position() - b->position();
    float distance = collision_vector.mag();
    if (distance == 0.f)
    {
        // Dynamic collision will drift these apart.
        return;
    }
    float overlap = .5f * (distance - static_cast<float>(rep(b->radius())) - static_cast<float>(rep(a->radius())));
    a->position() -= overlap * (collision_vector / distance);
    b->position() += overlap * (collision_vector / distance);
}

void PhysicsBall::dynamic_collision_response(PhysicsBall* a, PhysicsBall* b)
{
    auto collision_vector = b->position() - a->position();
    float distance = collision_vector.mag();
    if (distance == .0f)
    {
        distance = .1f;
    }
    auto collision_normal = collision_vector / distance;
    auto relative_velocity = a->velocity() - b->velocity();
    float speed = relative_velocity.dot(collision_normal);
    // These objects are moving away from each other already.
    if (speed < 0)
    {
        return;
    }
    float impulse = 2 * speed / (rep(a->weight()) + rep(b->weight()));
    a->velocity() -= collision_normal * impulse * static_cast<float>(rep(b->weight()));
    b->velocity() += collision_normal * impulse * static_cast<float>(rep(a->weight()));
}

bool PhysicsBall::collides_with(const PhysicsBall& a, const PhysicsBall& b)
{
    auto angle = a.position() - b.position();
    float distance_2 = angle.mag2();
    return distance_2 <= ((rep(a.radius()) + rep(b.radius())) * (rep(a.radius()) + rep(b.radius())));
}

auto PhysicsBall::impacts_world_bounds(const PhysicsBall& ball, const World& world) -> ImpactWorldResult
{
    using enum ImpactWorldResult;
    if (ball.position().x + rep(ball.radius()) > rep(world.width()))
    {
        return Right;
    }
    if (ball.position().x - rep(ball.radius()) < 0)
    {
        return Left;
    }
    if (ball.position().y + rep(ball.radius()) > rep(world.height()))
    {
        return Bottom;
    }
    if (ball.position().y - rep(ball.radius()) < 0)
    {
        return Top;
    }
    return None;
}

void PhysicsBall::world_collision_response(ImpactWorldResult result, PhysicsBall* ball, const World& world)
{
    assert(impacts_world_bounds(*ball, world) == result && result != ImpactWorldResult::None);
    PhysicsPoint normal{ };
    switch (result)
    {
    case ImpactWorldResult::Left:
        normal = { 1.f, 0.f };
        ball->position().x += -(ball->position().x - rep(ball->radius()));
        break;
    case ImpactWorldResult::Right:
        normal = { -1.f, 0.f };
        ball->position().x += rep(world.width()) - (ball->position().x + rep(ball->radius()));
        break;
    case ImpactWorldResult::Top:
        normal = { 0.f, -1.f };
        ball->position().y += -(ball->position().y - rep(ball->radius()));
        break;
    case ImpactWorldResult::Bottom:
        normal = { 0.f, 1.f };
        ball->position().y += rep(world.height()) - (ball->position().y + rep(ball->radius()));
        break;
    default:
        return;
    }
    float d = 2 * ball->velocity().dot(normal);
    ball->velocity() -= (normal * d) * ball->properties().friction;
}