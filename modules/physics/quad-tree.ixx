module;
#include <array>
#include <concepts>
#include <memory>
#include <utility>
#include <vector>
export module Physics.QuadTree;

import Physics.Ball;
import Physics.Utils;
import Util.BasicTypes;
import Util.EnumUtils;

export
enum class Level : int { };

export
class QuadTree
{
    static constexpr int max_depth = 5;
    static constexpr int split_factor = 20;
public:
    using BoundingBox = Box;

    QuadTree(const PixelPoint& upper_left, Width width, Height height, Level level):
        rect{ upper_left, width, height }, level{ level } { }

    void clear()
    {
        objects.clear();
        trees = { }; // clear the existing trees.
    }

    void insert(PhysicsBall* ball)
    {
        internal_insert(box_for(*ball), ball);
    }

    template <std::invocable<PhysicsBall*> F>
    void for_each_in(const BoundingBox& box, F&& invocable) const
    {
        int i = index(box);
        if (i != -1 && trees[i])
        {
            trees[i]->for_each_in(box, std::forward<F>(invocable));
        }
        for (PhysicsBall* ball : objects)
        {
            std::forward<F>(invocable)(ball);
        }
    }

    static BoundingBox box_for(const PhysicsBall& ball)
    {
        int r = rep(ball.radius());
        return { ball.position(), Width(r), Height(r) };
    }

    std::vector<BoundingBox> all_boxes() const
    {
        std::vector<BoundingBox> boxes;
        internal_all_boxes(&boxes);
        return boxes;
    }
private:
    void internal_all_boxes(std::vector<BoundingBox>* boxes) const
    {
        boxes->push_back(rect);

        for (const auto& tree : trees)
        {
            if (tree)
            {
                tree->internal_all_boxes(boxes);
            }
        }
    }

    void internal_insert(const BoundingBox& box, PhysicsBall* ball)
    {
        if (trees[0])
        {
            int i = index(box);
            if (i != -1)
            {
                trees[i]->internal_insert(box, ball);
                return;
            }
        }

        objects.push_back(ball);

        if (objects.size() > split_factor && rep(level) < max_depth)
        {
            if (!trees[0])
            {
                split();
            }

            int i = 0;
            while (i < static_cast<int>(objects.size()))
            {
                BoundingBox object_box = box_for(*objects[i]);
                int idx = index(object_box);
                if (idx != -1)
                {
                    trees[idx]->internal_insert(object_box, objects[i]);
                    objects.erase(begin(objects) + i);
                }
                else
                {
                    ++i;
                }
            }
        }
    }

    void split()
    {
        int sub_width = rep(rect.width()) / 2;
        int sub_height = rep(rect.height()) / 2;
        int x = rect.left();
        int y = rect.top();
        int next_level = rep(level) + 1;

        trees[0] = std::make_unique<QuadTree>(PixelPoint{ x + sub_width, y },              Width(sub_width), Height(sub_height), Level(next_level));
        trees[1] = std::make_unique<QuadTree>(PixelPoint{ x,             y },              Width(sub_width), Height(sub_height), Level(next_level));
        trees[2] = std::make_unique<QuadTree>(PixelPoint{ x,             y + sub_height }, Width(sub_width), Height(sub_height), Level(next_level));
        trees[3] = std::make_unique<QuadTree>(PixelPoint{ x + sub_width, y + sub_height }, Width(sub_width), Height(sub_height), Level(next_level));
    }

    int index(const BoundingBox& box) const
    {
        int index = -1;
        float vert_mid = rect.left() + static_cast<float>(rep(rect.width())) / 2.f;
        float horiz_mid = rect.top() + static_cast<float>(rep(rect.height())) / 2.f;

        auto is_top = [&]
        {
            return box.top() < horiz_mid && box.bottom() < horiz_mid;
        };
        auto is_bottom = [&]
        {
            return box.top() > horiz_mid;
        };

        if (box.left() <= vert_mid && box.right() <= vert_mid)
        {
            if (is_top())
            {
                index = 1;
            }
            else if (is_bottom())
            {
                index = 2;
            }
        }
        else if (box.left() >= vert_mid)
        {
            if (is_top())
            {
                index = 0;
            }
            else if (is_bottom())
            {
                index = 3;
            }
        }

        return index;
    }

    std::array<std::unique_ptr<QuadTree>, max_depth> trees;
    std::vector<PhysicsBall*> objects;
    BoundingBox rect;
    const Level level;
};