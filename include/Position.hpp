#pragma once

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
