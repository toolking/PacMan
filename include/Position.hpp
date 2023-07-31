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

inline constexpr auto coord_to_position(int x, int y) -> cen::ipoint
{
    return cen::ipoint {x * BLOCK_SIZE_24 + BLOCK_SIZE_24 / 2, y * BLOCK_SIZE_24 + BLOCK_SIZE_24 / 2};
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