#ifndef PHYSICS_BALL_H
#define PHYSICS_BALL_H

#include "physics/physics-utils.h"
#include "util/basic-types.h"
#include "util/enum-utils.h"
#include "world/world.h"

struct PhysicalProperties
{
    float stickyness = 150.f;
    float friction = .85f;
};

class PhysicsBall
{
public:
    PhysicsBall(const PhysicsPoint& pos,
                 const PhysicsPoint& velocity,
                 Color color,
                 Radius radius,
                 Weight weight = Weight(1),
                 PhysicalProperties properties = { }):
        pos{ pos },
        vel{ velocity },
        pixel_color{ color },
        r{ radius },
        w{ weight },
        props{ properties } { }

    void dead(bool b)
    {
        exploded = b;
    }

    bool dead() const
    {
        return exploded;
    }

    const PhysicsPoint& position() const
    {
        return pos;
    }

    PhysicsPoint& position()
    {
        return pos;
    }

    PhysicsPoint& old_position()
    {
        return prev_pos;
    }

    const PhysicsPoint& old_position() const
    {
        return prev_pos;
    }

    const PhysicsPoint& velocity() const
    {
        return vel;
    }

    PhysicsPoint& velocity()
    {
        return vel;
    }

    Color color() const
    {
        return pixel_color;
    }

    Radius radius() const
    {
        return r;
    }

    Weight weight() const
    {
        return w;
    }

    const PhysicalProperties& properties() const
    {
        return props;
    }

    float physics_time_remaining() const
    {
        return time_remaining;
    }

    void physics_time_remaining(float dt)
    {
        time_remaining = dt;
    }

    bool single_point() const
    {
        return rep(radius()) == 0;
    }

    AABBBox bounding_box() const
    {
        return { position(), radius() };
    }

    static void static_collision_response(PhysicsBall* a, PhysicsBall* b);

    static void dynamic_collision_response(PhysicsBall* a, PhysicsBall* b);

    static bool collides_with(const PhysicsBall& a, const PhysicsBall& b);

    enum class ImpactWorldResult
    {
        None,
        Left,
        Right,
        Top,
        Bottom
    };

    static ImpactWorldResult impacts_world_bounds(const PhysicsBall& a, const World& world);

    static void world_collision_response(ImpactWorldResult result, PhysicsBall* ball, const World& world);

private:
    bool can_stick() const
    {
        return velocity().mag2() < (props.stickyness * props.stickyness);
    }

    Color pixel_color;
    PhysicsPoint pos;
    PhysicsPoint vel;
    Radius r;
    Weight w;
    PhysicsPoint prev_pos = pos;
    PhysicalProperties props;
    float time_remaining = 0.f;
    bool exploded = false;
};

#endif // PHYSICS_BALL_H