#pragma once

#include "Globals.hpp"

#include <compare>

class Position
{
public:
    Position() = default;
    Position(short x, short y)
      : x {x}, y {y}
    {}
    std::strong_ordering operator<=>(Position const& other) const = default;
    short x{}, y{};
};

inline
bool is_colliding(Position const& a, Position const& b)
{
    return (a.x > b.x - BLOCK_SIZE_24 && a.x < b.x + BLOCK_SIZE_24)
        && (a.y > b.y - BLOCK_SIZE_24 && a.y < b.y + BLOCK_SIZE_24);
}
