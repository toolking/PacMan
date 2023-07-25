#pragma once

#include "Board.hpp"
#include "Entity.hpp"
#include "Globals.hpp"
#include "Position.hpp"

class Board;

class Entity
{
public:
    enum class Type : unsigned char
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

    unsigned char facing() const
    {
        return facing_;
    }

    bool is_alive() const
    {
        return life_statement_;
    }

    Position position() const
    {
        return position_;
    }

    void speed(unsigned char speed)
    {
        speed_ = speed;
    }

    void direction(Direction direction)
    {
        direction_ = direction;
    }

    void facing(unsigned char facing)
    {
        facing_ = facing;
    }

    void life_statement(bool life_statement)
    {
        life_statement_ = life_statement;
    }

    void position(Position const& position)
    {
        position_ = position;
    }

    bool is_colliding(Position const& other)
    {
        return (other.x > position_.x - BOCK_SIZE_24 && other.x < position_.x + BOCK_SIZE_24)
            && (other.y > position_.y - BOCK_SIZE_24 && other.y < position_.y + BOCK_SIZE_24);
    }

protected:
    void move(Direction mover);
    void check_wrap();
    void get_possible_position(short& x, short& y, Direction mover);
    bool wall_collision(short x, short y, Board::board_type const& actual_map, bool can_use_door = false);
    auto char_board_pos(unsigned char side_dir, float cell_x, float cell_y) -> Position;

private:
    Type identity_;
    unsigned char speed_ {2};
    unsigned char facing_ {};
    bool life_statement_ {true};
    Position position_ {};
    Direction direction_ {Direction::Right};
};

inline auto entity_type_2_char(Entity::Type e) -> char
{
    switch (e) {
        using enum Entity::Type;
    case PacMan:
        return '0';
    case Blinky:
        return '1';
    case Inky:
        return '2';
    case Pinky:
        return '3';
    case Clyde:
        return '4';
    default:
        return ' ';
    }
}