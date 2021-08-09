module;
#pragma warning(push)
#pragma warning(disable: 4201) // nonstandard extension used: nameless struct/union
#pragma warning(disable: 4245) // 'argument': conversion from 'int' to 'uint8_t', possible loss of data
#include "olcPixelGameEngine.h"
#pragma warning(pop)
export module Bridges.PGE;

export
namespace olc
{
    // For game.
    using olc::PixelGameEngine;
    using olc::Key;

    // Note: Because these color constants are defined to be static in the header they cannot be
    // directly exported.  Instead we export their values through a module-owned variable.
    namespace ModuleColors
    {
        auto Black()
        {
            return olc::BLACK;
        }

        auto White()
        {
            return olc::WHITE;
        }

        auto Blue()
        {
            return olc::BLUE;
        }

        auto Red()
        {
            return olc::RED;
        }
    }

    // For basic types.
    using olc::Pixel;
    using olc::vf2d;
    using olc::vi2d;

    // Allow using the multiply operator from olc::v2d_generic.
    using olc::operator*;
}