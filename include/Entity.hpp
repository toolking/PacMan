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

    unsigned char speed() const
    {
        return speed_;
    }

    Direction direction() const
    {
        return direction_;
    }

    unsigned int facing() const
    {
        return facing_;
    }

    bool is_alive() const
    {
        return life_statement_;
    }

    void speed(unsigned char speed)
    {
        speed_ = speed;
    }

    void direction(Direction direction)
    {
        direction_ = direction;
    }

    void facing(unsigned int facing)
    {
        facing_ = facing;
    }

    void life_statement(bool life_statement)
    {
        life_statement_ = life_statement;
    }

    Position position {};

protected:
    void move(Direction mover);
    void check_wrap();
    void get_possible_position(short& x, short& y, Direction mover);
    bool wall_collision(short x, short y, Board::board_type const& actual_map, bool can_use_door = false);
    auto char_board_pos(unsigned char side_dir, float cell_x, float cell_y) -> Position;

private:
    Type identity_;
    unsigned int speed_ {2};
    unsigned int facing_ {};
    bool life_statement_ {true};
    Direction direction_ {Direction::Right};
};
