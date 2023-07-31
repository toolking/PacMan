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
      , start_position_ {entity_start_position(identity_)}
    {}

    Type identity() const
    {
        return identity_;
    }

    cen::ipoint position {};
    unsigned int speed {2};
    bool is_alive {true};
    Direction direction {Direction::Right};
    unsigned int facing {};

    static constexpr auto entity_start_position(Type const& e) -> cen::ipoint
    {
        using enum Entity::Type;
        char const c = (e == PacMan) ? '0'
            : (e == Blinky)          ? '1'
            : (e == Inky)            ? '2'
            : (e == Pinky)           ? '3'
            : (e == Clyde)           ? '4'
                                     : ' ';
        auto const i = Board::CHAR_BOARD.find(c);
        unsigned char const x = i % BOARD_WIDTH;
        unsigned char const y = i / BOARD_WIDTH;
        return cen::ipoint(x * BLOCK_SIZE_24 + BLOCK_SIZE_24 / 2, y * BLOCK_SIZE_24);
    }

    void reset()
    {
        position = start_position_;
        is_alive = true;
        direction = Direction::Nowhere;
        facing = 0;
    }

protected:
    void move(Direction mover);
    void check_wrap();
    auto get_possible_position(cen::ipoint const& position, Direction mover) -> cen::ipoint;
    bool wall_collision(cen::ipoint const& pos, board_type const& actual_map, bool can_use_door = false);
    auto char_board_pos(unsigned char side_dir, cen::fpoint const& pos) -> cen::ipoint;

private:
    Type identity_;
    cen::ipoint const start_position_ {};
};
