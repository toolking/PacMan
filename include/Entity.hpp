#pragma once

#include "Board.hpp"
#include "Entity.hpp"
#include "Globals.hpp"
#include "Position.hpp"

class Board;

class Entity
{
public:
    enum class Type
    {
        Noone = 0,
        PacMan,
        Blinky,
        Inky,
        Pinky,
        Clyde
    };

    Entity(Type identity)
      : identity_ {identity}
    {}

    Type identity() const
    {
        return identity_;
    }

    unsigned int facing() const
    {
        return facing_;
    }

    void facing(unsigned int facing)
    {
        facing_ = facing;
    }

    Position position {};
    unsigned int speed {2};
    bool is_alive {true};
    Direction direction {Direction::Right};

protected:
    void move(Direction mover);
    void check_wrap();
    auto get_possible_position(Position const& position, Direction mover) -> Position;
    bool wall_collision(Position const& pos, board_type const& actual_map, bool can_use_door = false);
    auto char_board_pos(unsigned char side_dir, float cell_x, float cell_y) -> Position;

private:
    Type identity_;
    unsigned int facing_ {};
};
