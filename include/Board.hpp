#pragma once

#include "Globals.hpp"
#include "Texture.hpp"
#include "Timer.hpp"
#include "Position.hpp"

#include <array>

class Entity;

enum class BlockType
{
    Nothing=0,
    Wall,
    Door,
    Pellet,
    Energizer
};

class Board
{
public:

    using block_type = BlockType;
    using board_type = std::array<block_type, BOARD_HEIGHT * BOARD_WIDTH>;

    Board();
    auto map() const -> Board::board_type;
    auto entity_start_position(Entity const&) const -> Position; 
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
    Texture score_label_texture_;
    Texture score_texture_;
    Texture high_score_label_texture_;
    Texture high_score_texture_;
    board_type numeric_board_;
    unsigned int score_ {};
    bool is_extra_ {};
    unsigned int lives_ {4};
};
