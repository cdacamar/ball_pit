#include "physics/physics-engine.h"

#include <cassert>

#include "util/stopwatch.h"

void PhysicsEngine::update(float dt, World* world)
{
    constexpr int updates = 4;
    float sim_elapsed_time = dt / updates;
    constexpr int max_steps = 4;
    int dead_count = 0;
    dynamic_collision_pairs.reserve(ball_collection.size());

    // Track timing.
    init_timer_collections(updates * max_steps);
    Stopwatch stopwatch_all;
    Stopwatch stopwatch;

    for (int i = 0; i != updates; ++i)
    {
        stopwatch_all.start();
        for (PhysicsBall& ball : ball_collection)
        {
            ball.physics_time_remaining(sim_elapsed_time);
        }

        dead_count = 0;
        bool early_break = true;
        for (int j = 0; j != max_steps; ++j)
        {
            clear_quadtree(world);

            for (PhysicsBall& ball : ball_collection)
            {
                if (ball.dead())
                {
                    ++dead_count;
                    continue;
                }

                // This ball has exhausted its maximum allotted time for this
                // epoch.
                if (ball.physics_time_remaining() <= 0.f)
                {
                    continue;
                }
                early_break = false;

                ball.old_position() = ball.position();

                // Add gravity.
                ball.velocity().y += gravity_constant * ball.physics_time_remaining();

                if (ball.velocity().mag2() < 0.005f)
                {
                    ball.velocity() = { };
                }

                // Always add x velocity.
                ball.position() += ball.velocity() * ball.physics_time_remaining();

                auto result = PhysicsBall::impacts_world_bounds(ball, *world);
                if (result != PhysicsBall::ImpactWorldResult::None)
                {
                    PhysicsBall::world_collision_response(result, &ball, *world);
                }
            }

            if (early_break)
            {
                break;
            }

            // If we are doing extra interactions, do them.
            stopwatch.start();
            for (PhysicsBall& ball : ball_collection)
            {
                if (!ball.dead())
                {
                    quad_tree->insert(&ball);
                }
            }
            stopwatch.stop();
            current_tree_build_times.push_back(stopwatch.to_ms());

            stopwatch.start();
            dynamic_collision_pairs.clear();
            for (PhysicsBall& ball : ball_collection)
            {
                if (ball.dead())
                {
                    continue;
                }
                intersect_objects_static_response(&ball);
                float intended_speed = ball.velocity().mag();
                //float intended_distance = intended_speed * ball.physics_time_remaining();
                float actual_distance = (ball.position() - ball.old_position()).mag();
                float actual_time = 0.f;
                if (intended_speed > 0.f)
                {
                    actual_time = actual_distance / intended_speed;
                }
                ball.physics_time_remaining(ball.physics_time_remaining() - actual_time);
            }

            dynamic_collision_responses();

            stopwatch.stop();
            current_collision_times.push_back(stopwatch.to_ms());
        }
        stopwatch_all.stop();
        current_update_times.push_back(stopwatch_all.to_ms());
    }

    constexpr int cull_dead_threshold = 50;
    if (dead_count >= cull_dead_threshold)
    {
        std::erase_if(ball_collection,
                        [](const PhysicsBall& ball)
                        {
                            return ball.dead();
                        });
    }
}

void PhysicsEngine::init_timer_collections(int steps)
{
    current_update_times.clear();
    current_tree_build_times.clear();
    current_collision_times.clear();
    int reserve_quantity = std::max(0, steps); // 'steps' can be negative.
    current_update_times.reserve(reserve_quantity);
    current_tree_build_times.reserve(reserve_quantity);
    current_collision_times.reserve(reserve_quantity);
}

void PhysicsEngine::clear_quadtree(const World* engine)
{
    quad_tree = nullptr;
    quad_tree = std::make_unique<QuadTree>(PixelPoint{ 0, 0 }, engine->width(), engine->height(), Level(0));
}

void PhysicsEngine::intersect_objects_static_response(PhysicsBall* ball)
{
    assert(quad_tree != nullptr);
    quad_tree->for_each_in(QuadTree::box_for(*ball),
                        [&](PhysicsBall* other)
                        {
                            if (other == ball)
                            {
                                return;
                            }

                            if (other->dead())
                            {
                                return;
                            }

                            if (overlap_AABB(ball->bounding_box(), other->bounding_box())
                                && PhysicsBall::collides_with(*ball, *other))
                            {
                                PhysicsBall::static_collision_response(ball, other);
                                dynamic_collision_pairs.emplace_back(ball, other);
                            }
                        });
}

void PhysicsEngine::dynamic_collision_responses()
{
    for (auto [ball_a, ball_b] : dynamic_collision_pairs)
    {
        if (!ball_a->dead() && !ball_b->dead())
        {
            PhysicsBall::dynamic_collision_response(ball_a, ball_b);
        }
    }
}