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

constexpr unsigned short SCORE_TABLE[FRUIT_FRAMES] = {100, 300, 500, 700, 1000, 2000, 3000, 5000};

constexpr auto BLACK = cen::colors::black;
constexpr auto WHITE = cen::colors::white;
constexpr auto YELLOW = cen::colors::yellow;
constexpr auto RED = cen::colors::red;
constexpr auto CYAN = cen::colors::cyan;
constexpr auto PINK = cen::colors::pink;
constexpr auto ORANGE = cen::colors::orange;
constexpr auto BLUE = cen::colors::blue;

enum class Direction : unsigned char
{
    Nowhere=0,
    Right,
    Up,
    Left,
    Down
};

inline Direction operator- (Direction const& dir)
{
    switch (dir) {
    case Direction::Right:
        return Direction::Left;
    case Direction::Up:
        return Direction::Down;
    case Direction::Left:
        return Direction::Right;
    case Direction::Down:
        return Direction::Up;
    default:
        return Direction::Nowhere;
    }
}

template<std::size_t Size>
inline
auto init_frames() -> auto
{
    std::array<cen::irect, Size> sprite_clips;
    unsigned short x = 0;
    for (auto& sprite : sprite_clips) {
        sprite.set_position(x, 0);
        sprite.set_size(BLOCK_SIZE_32, BLOCK_SIZE_32);
        x += BLOCK_SIZE_32;
    }
    return sprite_clips;
}
