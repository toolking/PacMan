#pragma once

#include <compare>

class Position
{
public:
    explicit Position(short x = 0, short y = 0)
      : x {x}, y {y}
    {}
    std::strong_ordering operator<=>(Position const& other) const = default;
    short x{}, y{};
};
