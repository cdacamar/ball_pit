#include "bridges/pge-bridge.h"

#include "physics/physics-ball.h"
#include "physics/physics-engine.h"
#include "physics/quad-tree.h"
#include "util/basic-types.h"
#include "util/enum-utils.h"
#include "util/random-generator.h"
#include "world/world.h"

class BallPit : olc::PixelGameEngine
{
    using Base = olc::PixelGameEngine;

public:
    BallPit()
    {
        sAppName = "Ball Pit";
    }

    bool OnUserCreate() override
    {
        return true;
    }

    bool OnUserUpdate(float dt) override
    {
        bool quit = false;
        bool clear = false;
        bool show_help = false;
        // Poll input
        if (GetKey(olc::Key::ESCAPE).bReleased)
        {
            quit = true;
        }

        if (GetKey(olc::Key::TAB).bHeld)
        {
            show_help = true;
        }

        if (GetKey(olc::Key::C).bReleased)
        {
            clear = true;
        }

        // Toggle showing the quad tree.
        if (GetKey(olc::Key::Q).bReleased)
        {
            should_draw_quad_tree = !should_draw_quad_tree;
        }

        if (GetMouse(0).bHeld)
        {
            drop();
        }

        // Update
        if (clear)
        {
            physics_engine.remove_all();
        }
        physics_engine.update(dt, &world);

        // Draw
        Clear(olc::BLACK);

        // Let anything overlap this.
        DrawString({ 10, rep(world.height()) - 10 }, "TAB to show help");

        draw_physics();

        if (show_help)
        {
            draw_help();
        }

        return !quit;
    }

    auto Construct(ScreenInfo info)
    {
        screen_info = info;
        world.set(info.width, info.height);
        return Base::Construct(rep(info.width), rep(info.height), rep(info.px_width), rep(info.px_height), false, true);
    }

    using Base::Start;
private:
    PixelPoint mouse_position() const
    {
        return { GetMouseX(), GetMouseY() };
    }

    void drop()
    {
        RealDistribution<float> dis_velocity_x{ -1500.f, 1500.f };
        RealDistribution<float> dis_velocity_y{ -500.f, 500.f };
        RealDistribution<float> dis_stickyness{ 0.f, 0.f };
        RealDistribution<float> dis_friction{ .1f, .85f };
        IntDistribution<int> dis_color{ 0, 255 };
        IntDistribution<int> dis_radius{ 2, 10 };
        for (int i = 0; i != 10; ++i)
        {
            PhysicsPoint velocity{ random_generator().generate(dis_velocity_x), random_generator().generate(dis_velocity_y) };
            Color color {
                static_cast<uint8_t>(random_generator().generate(dis_color)),
                static_cast<uint8_t>(random_generator().generate(dis_color)),
                static_cast<uint8_t>(random_generator().generate(dis_color)) };
            auto radius = random_generator().generate(dis_radius);
            auto weight = radius * 2 + 1;
            physics_engine.add(PhysicsBall{ { mouse_position() },
                                                velocity,
                                                color,
                                                Radius(radius),
                                                Weight(weight),
                                                { .stickyness = random_generator().generate(dis_stickyness),
                                                    .friction = random_generator().generate(dis_friction) } });
        }
    }

    void draw_help()
    {
        constexpr int num_options = 5;
        constexpr int text_height = 10;
        constexpr int total_height = 5 * 2 + num_options * text_height;
        constexpr int starting_point = 20;

        // Background
        FillRect({ 10, 10 }, { rep(world.width()) - starting_point, total_height + 10 }, olc::BLUE);

        PixelPoint pos = { starting_point, starting_point };
        // Quit
        DrawString(pos, "ESC: Quit");
        pos.y += text_height;

        // Help
        DrawString(pos, "TAB: Show this help");
        pos.y += text_height;

        // Clear balls
        DrawString(pos, "C: Clear balls");
        pos.y += text_height;

        // Draw quad tree
        DrawString(pos, "Q: Draw quad trees");
        pos.y += text_height;

        // Drop balls
        DrawString(pos, "Mouse 1: Drop balls!");
    }

    void draw_physics()
    {
        int alive_balls = 0;
        for (const PhysicsBall& ball : physics_engine.objects())
        {
            if (!ball.dead())
            {
                ++alive_balls;

                if (ball.single_point())
                {
                    Draw(ball.position(), ball.color());
                }
                else
                {
                    FillCircle(ball.position(), rep(ball.radius()), ball.color());
                }
            }
        }

        if (should_draw_quad_tree)
        {
            draw_quad_tree();
        }

        DrawString({ 10, 10 }, std::to_string(alive_balls));
    }

    void draw_quad_tree()
    {
        if (const QuadTree* tree = physics_engine.current_quad_tree())
        {
            auto boxes = tree->all_boxes();
            for (const QuadTree::BoundingBox& box : boxes)
            {
                const Color color = olc::RED;
                // Top line
                DrawLine({ box.left(), box.top() }, { box.right(), box.top() }, color);
                // Right line
                DrawLine({ box.right(), box.top() }, { box.right(), box.bottom() }, color);
                // Bottom line
                DrawLine({ box.right(), box.bottom() }, { box.left(), box.bottom() }, color);
                // Left line
                DrawLine({ box.left(), box.bottom() }, { box.left(), box.top() }, color);
            }
        }
    }

    ScreenInfo screen_info;
    World world;
    PhysicsEngine physics_engine;

    bool should_draw_quad_tree = false;
};

int main()
{
    BallPit ball_pit_game;
    if (ball_pit_game.Construct({ Width(640), Height(480), PixelWidth(2), PixelHeight(2) }))
    {
        ball_pit_game.Start();
    }
}