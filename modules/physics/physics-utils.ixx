export module Physics.Utils;

import Util.BasicTypes;
import Util.EnumUtils;

export
{

struct AABBBox
{
    PixelPoint center = { };
    Radius radius = { };
};

bool overlap_AABB(const AABBBox& first, const AABBBox& second)
{
    return first.center.x + rep(first.radius) + rep(second.radius) > second.center.x
        && first.center.x < second.center.x + rep(first.radius) + rep(second.radius)
        && first.center.y + rep(first.radius) + rep(second.radius) > second.center.y
        && first.center.y < second.center.y + rep(first.radius) + rep(second.radius);
}

class Box
{
public:
    Box(const PixelPoint& upper_left, Width width, Height height):
        upper_left{ upper_left }, w{ width }, h{ height } { }

    int left() const
    {
        return upper_left.x;
    }

    int top() const
    {
        return upper_left.y;
    }

    int bottom() const
    {
        return top() + rep(height());
    }

    int right() const
    {
        return left() + rep(width());
    }

    Width width() const
    {
        return w;
    }

    Height height() const
    {
        return h;
    }
private:
    PixelPoint upper_left;
    Width w;
    Height h;
};

bool point_in(const PixelPoint& point, const Box& box)
{
    return box.left() + rep(box.width()) > point.x
        && box.left() <= point.x
        && box.top() + rep(box.height()) > point.y
        && box.top() <= point.y;
}

} // export