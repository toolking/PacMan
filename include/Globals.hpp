#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include <string>
#include <vector>

constexpr unsigned char BOARD_WIDTH = 28;
constexpr unsigned char BOARD_HEIGHT = 36;
constexpr unsigned char BLOCK_SIZE_32 = 32;
constexpr unsigned char BOCK_SIZE_24 = 24;
constexpr unsigned short WINDOW_WIDTH = BOARD_WIDTH * BOCK_SIZE_24;
constexpr unsigned short WINDOW_HEIGHT = BOARD_HEIGHT * BOCK_SIZE_24;

constexpr unsigned char LIVING_PAC_FRAMES = 3;
constexpr unsigned char DEATH_PAC_FRAMES = 10;
constexpr unsigned char GHOST_BODY_FRAMES = 6;
constexpr unsigned char GHOST_EYE_FRAMES = 5;
constexpr unsigned char FRUIT_FRAMES = 8;

constexpr unsigned short SCORE_TABLE[FRUIT_FRAMES] = {100, 300, 500, 700, 1000, 2000, 3000, 5000};

constexpr SDL_Color BLACK = { .r = 0, .g = 0, .b = 0, .a = 0};
constexpr SDL_Color WHITE = { .r = 255, .g = 255, .b = 255, .a = 0};
constexpr SDL_Color YELLOW = { .r = 255, .g = 255, .b = 0, .a = 0};
constexpr SDL_Color RED = { .r = 255, .g = 0, .b = 0, .a = 0};
constexpr SDL_Color CYAN = { .r = 0, .g = 192, .b = 255, .a = 0};
constexpr SDL_Color PINK = { .r = 255, .g = 192, .b = 203, .a = 0};
constexpr SDL_Color ORANGE = { .r = 255, .g = 128, .b = 0, .a = 0};

extern SDL_Window* Window;
extern SDL_Renderer* Renderer;
extern TTF_Font* Font;
extern TTF_Font* LittleFont;

enum class Direction : unsigned char
{
    Nowhere=0,
    Right,
    Up,
    Left,
    Down
};

enum BlockType : unsigned char
{
    Wall,
    Door,
    Pellet,
    Energizer,
    Nothing
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

void initialize_SDL();
void close_SDL();

template<std::size_t Size>
inline
void init_frames(
    std::array<SDL_Rect, Size>& sprite_clips
){
    unsigned short x = 0;
    for (auto& sprite : sprite_clips) {
        sprite.x = x;
        sprite.y = 0;
        sprite.w = BLOCK_SIZE_32;
        sprite.h = BLOCK_SIZE_32;
        x += BLOCK_SIZE_32;
    }
}
