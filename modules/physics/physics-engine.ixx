module;
#include <chrono>
#include <memory>
#include <vector>
export module Physics.Engine;

import Physics.Ball;
import Physics.QuadTree;
import World;

export
class PhysicsEngine
{
    static constexpr int d_time = 16;
    static constexpr float d_time_s = static_cast<float>(d_time) / 1000.f;
    static constexpr float gravity_constant = 980.f;
public:
    void add(const PhysicsBall& ball)
    {
        ball_collection.push_back(ball);
    }

    void update(float dt, World* world);

    void remove_all()
    {
        ball_collection.clear();
    }

    const auto& objects() const
    {
        return ball_collection;
    }

    const QuadTree* current_quad_tree() const
    {
        return quad_tree.get();
    }

    const auto& update_times() const
    {
        return current_update_times;
    }

    const auto& tree_build_times() const
    {
        return current_tree_build_times;
    }

    const auto& collision_times() const
    {
        return current_collision_times;
    }
private:
    void init_timer_collections(int steps);

    void clear_quadtree(const World* world);

    void intersect_objects_static_response(PhysicsBall* ball);

    void dynamic_collision_responses();

    std::vector<PhysicsBall> ball_collection;
    std::unique_ptr<QuadTree> quad_tree;
    std::vector<std::pair<PhysicsBall*, PhysicsBall*>> dynamic_collision_pairs;
    std::vector<std::chrono::milliseconds> current_update_times;
    std::vector<std::chrono::milliseconds> current_tree_build_times;
    std::vector<std::chrono::milliseconds> current_collision_times;
};