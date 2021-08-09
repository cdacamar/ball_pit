module;
#include <cstdint>
export module Util.BasicTypes;

import Bridges.PGE;

export
{

enum class Width : std::int32_t { };
enum class Height : std::int32_t { };
enum class Radius : std::int32_t { };
enum class Weight : std::int32_t { };
enum class PixelWidth : std::int32_t { };
enum class PixelHeight : std::int32_t { };
enum class Row : std::int32_t { };
enum class Column : std::int32_t { };

struct ScreenInfo
{
    Width width;
    Height height;
    PixelWidth px_width;
    PixelHeight px_height;
};

using PixelPoint = olc::vi2d;
using PhysicsPoint = olc::vf2d;
using Color = olc::Pixel;

} // export