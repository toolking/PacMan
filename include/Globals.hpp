#pragma once

#include <centurion.hpp>

#include <string>
#include <vector>

constexpr unsigned short MIN_FPS = 90;
constexpr unsigned long FRAME_DURATION_MS = 1000 / MIN_FPS;

constexpr unsigned short START_WAIT_TICKS = 4500u;
constexpr unsigned char BOARD_WIDTH = 28;
constexpr unsigned char BOARD_HEIGHT = 36;
constexpr unsigned char BLOCK_SIZE_32 = 32;
constexpr unsigned char BLOCK_SIZE_24 = 24;
constexpr unsigned short WINDOW_WIDTH = BOARD_WIDTH * BLOCK_SIZE_24;
constexpr unsigned short WINDOW_HEIGHT = BOARD_HEIGHT * BLOCK_SIZE_24;

constexpr unsigned char LIVING_PAC_FRAMES = 3;
constexpr unsigned char DEATH_PAC_FRAMES = 10;
constexpr unsigned char GHOST_BODY_FRAMES = 6;
constexpr unsigned char GHOST_EYE_FRAMES = 5;
constexpr unsigned char FRUIT_FRAMES = 8;

constexpr std::array<unsigned short,FRUIT_FRAMES> SCORE_TABLE = {100, 300, 500, 700, 1000, 2000, 3000, 5000};

enum class Direction : unsigned char
{
    Nowhere=0,
    Right,
    Up,
    Left,
    Down
};

constexpr inline auto wrapped_distance(cen::ipoint const& pos1, cen::ipoint const& pos2) -> float
{
    auto dist_x = abs(pos1.x() - pos2.x());
    dist_x = (dist_x > WINDOW_WIDTH / 2)?WINDOW_WIDTH - dist_x:dist_x;
    return sqrt(dist_x*dist_x + pow(pos1.y() - pos2.y(), 2));
}

inline Direction operator- (Direction const& dir)
{
    using enum Direction;
    switch (dir) {
    case Right: return Left;
    case Up: return Down;
    case Left: return Right;
    case Down: return Up;
    default: return Nowhere;
    }
}
