#pragma once

#include "Globals.hpp"

#include <compare>

inline constexpr auto is_colliding(cen::ipoint const& a_point, cen::ipoint const& b_point) -> bool
{
    cen::irect const b_rect {
        b_point.x() - BLOCK_SIZE_24, b_point.y() - BLOCK_SIZE_24,
        2 * BLOCK_SIZE_24, 2 * BLOCK_SIZE_24};
    return b_rect.contains(a_point);
}

inline constexpr auto operator*(cen::ipoint const& point, std::integral auto scalar) -> cen::ipoint
{
    return cen::ipoint {point.x() * scalar, point.y() * scalar};
}

inline constexpr auto operator*(std::integral auto scalar, cen::ipoint const& point) -> cen::ipoint
{
    return point * scalar;
}

inline constexpr auto operator/(cen::ipoint const& point, std::integral auto scalar) -> cen::ipoint
{
    return cen::ipoint {point.x() / scalar, point.y() / scalar};
}

inline constexpr auto coord_to_position(int x, int y) -> cen::ipoint
{
    return cen::ipoint {x,y} * BLOCK_SIZE_24 + cen::ipoint{BLOCK_SIZE_24 / 2, BLOCK_SIZE_24 / 2};
}

inline constexpr auto direction_to_point(Direction const& dir) -> cen::ipoint
{
    using enum Direction;
    switch (dir) {
    case Right: return cen::ipoint {1, 0};
    case Up: return cen::ipoint {0, -1};
    case Left: return cen::ipoint {-1, 0};
    case Down: return cen::ipoint {0, 1};
    default: return cen::ipoint {0, 0};
    }
}

inline constexpr auto operator+(cen::ipoint const& position, Direction move) -> cen::ipoint
{
    return position + direction_to_point(move);
}

inline constexpr auto operator*(cen::irect const& rect, std::integral auto scalar) -> cen::irect
{
    return cen::irect {rect.x() * scalar, rect.y() * scalar, rect.width() * scalar, rect.height() * scalar};
}
