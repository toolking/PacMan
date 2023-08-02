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

    inline
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
        return cen::ipoint {x, y} * BLOCK_SIZE_24 + cen::ipoint {BLOCK_SIZE_24 / 2, 0};
    }

    inline static constexpr auto char_board_pos(unsigned char side_dir, cen::fpoint const& cell_pos) -> cen::ipoint
    {
        switch (side_dir) {
        case 0: return {static_cast<int>(floor(cell_pos.x())), static_cast<int>(floor(cell_pos.y()))};
        case 1: return {static_cast<int>(ceil(cell_pos.x())), static_cast<int>(floor(cell_pos.y()))};
        case 2: return {static_cast<int>(floor(cell_pos.x())), static_cast<int>(ceil(cell_pos.y()))};
        case 3: return {static_cast<int>(ceil(cell_pos.x())), static_cast<int>(ceil(cell_pos.y()))};
        default: return {};
        }
    }

    inline static constexpr auto board_idx(cen::ipoint const& pos) -> std::size_t
    {
        return cen::cast<std::size_t>(BOARD_WIDTH * pos.y() + abs(pos.x()%BOARD_WIDTH));
    }

    inline static constexpr auto position_in_direction(cen::ipoint const& pos, Direction mover) -> cen::ipoint
    {
        return pos + direction_to_point(mover);
    }

    inline static constexpr auto wall_collision(cen::ipoint const& pos, board_type const& actual_map, bool can_use_door = false) -> bool
    {
        cen::fpoint const cell = pos.as_f();
        cen::fpoint const cell_pos = {cell.x() / BLOCK_SIZE_24, cell.y() / BLOCK_SIZE_24};
        for (unsigned char side_dir = 0; side_dir < 4; side_dir++) {
            auto const current_block_type = actual_map[board_idx(char_board_pos(side_dir, cell_pos))];
            if (BlockType::Wall == current_block_type) {
                return true;
            } else if (BlockType::Door == current_block_type) {
                return !can_use_door;
            }
        }
        return false;
    }

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

private:
    Type identity_;
    cen::ipoint const start_position_ {};
};
