export module World;

import Util.BasicTypes;
import Util.EnumUtils;

export
class World
{
public:
    void set(Width width, Height height)
    {
        world_width = width;
        world_height = height;
    }

    Width width() const
    {
        return world_width;
    }

    Height height() const
    {
        return world_height;
    }

    bool bounded(const PixelPoint& pos) const
    {
        return bounded(Row(pos.y), Column(pos.x));
    }

    bool bounded(Row row, Column col) const
    {
        if (rep(row) < 0)
        {
            return false;
        }

        if (rep(col) < 0)
        {
            return false;
        }

        return rep(row) < rep(height()) && rep(col) < rep(width());
    }
private:
    Width world_width = Width{};
    Height world_height = Height{};
};