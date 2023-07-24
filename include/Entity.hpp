#pragma once

#include "Board.hpp"
#include "Entity.hpp"
#include "Globals.hpp"
#include "Position.hpp"

enum EntityType : unsigned char
{
    ePacMan,
    eBlinky,
    eInky,
    ePinky,
    eClyde,
    Noone
};

class Board;

class Entity
{
public:
    Entity(EntityType identity)
      : identity_ {identity}
    {}

    unsigned char identity() const
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

    bool is_colliding(Position other)
    {
        return (other.x > position_.x - BOCK_SIZE_24 && other.x < position_.x + BOCK_SIZE_24) && (other.y > position_.y - BOCK_SIZE_24 && other.y < position_.y + BOCK_SIZE_24);
    }

protected:
    void move(Direction mover);
    void check_wrap();
    void get_possible_position(short& x, short& y, Direction mover);
    bool wall_collision(short x, short y, Board::board_type const& actual_map, bool can_use_door = false);
    void char_board_pos(unsigned char side_dir, Position& board_pos, float cell_x, float cell_y);

private:
    unsigned char identity_;
    unsigned char speed_ {2};
    Direction direction_ {Direction::Right};
    unsigned char facing_ {0};
    bool life_statement_ {true};
    Position position_ {Position {}};
};

inline auto entity_type_2_char(EntityType e) -> char
{
    switch (e) {
    case EntityType::ePacMan:
        return '0';
    case EntityType::eBlinky:
        return '1';
    case EntityType::eInky:
        return '2';
    case EntityType::ePinky:
        return '3';
    case EntityType::eClyde:
        return '4';
    default:
        return ' ';
    }
}