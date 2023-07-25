#pragma once

#include "Globals.hpp"
#include "Texture.hpp"
#include "Timer.hpp"

#include <array>

class Entity;

class Board
{
public:

    using block_type = unsigned char;
    using board_type = std::array<block_type, BOARD_HEIGHT * BOARD_WIDTH>;

    Board();
    void convert_sketch();
    void copy_board(board_type& actual_map);
    void reset_position(Entity& entity);
    void set_score();
    void set_high_score();
    bool is_extra_life();
    void increase_lives();
    void decrease_lives();
    void score_increase(unsigned short scorer);
    unsigned char get_lives();
    void draw(board_type const& actual_map, Timer map_animation_timer);

private:
    Texture map_texture_;
    Texture pellet_texture_;
    Texture energizer_texture_;
    Texture door_texture_;
    Texture lives_texture_;
    Texture score_word_texture_;
    Texture score_texture_;
    Texture high_score_word_texture_;
    Texture high_score_texture_;
    board_type numeric_board_;
    unsigned int score_ {0};
    bool is_extra_ {false};
    char lives_ {4};
};
