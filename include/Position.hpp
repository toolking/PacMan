#pragma once

#include "Globals.hpp"

#include <compare>

using Position = cen::ipoint;

//struct Position
//{
//    Position() = default;
//    Position(short x, short y)
//      : x {x}, y {y}
//    {}
//    std::weak_ordering operator<=>(Position const& other) const = default;
//    short x{}, y{};
//};

inline
bool is_colliding(Position const& a, Position const& b)
{
    return (a.x() > b.x() - BLOCK_SIZE_24 && a.x() < b.x() + BLOCK_SIZE_24)
        && (a.y() > b.y() - BLOCK_SIZE_24 && a.y() < b.y() + BLOCK_SIZE_24);
}

constexpr auto coord_to_position(short x, short y) -> Position
{
    return Position {x * BLOCK_SIZE_24 + BLOCK_SIZE_24 / 2, y * BLOCK_SIZE_24 + BLOCK_SIZE_24 / 2};
}