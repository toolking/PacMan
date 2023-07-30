#include "Entity.hpp"
#include "Globals.hpp"

auto Entity::get_possible_position(Position const& position, Direction mover) -> Position
{
    using enum Direction;
    switch (mover) {
    case Right: return position + Position{1, 0};
    case Up: return position + Position{0, -1};
    case Left: return position + Position{-1, 0};
    case Down: return position + Position{0, 1};
    default: return position;
    }
}

auto Entity::char_board_pos(unsigned char side_dir, cen::fpoint const& cell_pos) -> Position
{
    switch (side_dir) {
    case 0: return {static_cast<short>(floor(cell_pos.x())), static_cast<short>(floor(cell_pos.y()))};
    case 1: return {static_cast<short>(ceil(cell_pos.x())), static_cast<short>(floor(cell_pos.y()))};
    case 2: return {static_cast<short>(floor(cell_pos.x())), static_cast<short>(ceil(cell_pos.y()))};
    case 3: return {static_cast<short>(ceil(cell_pos.x())), static_cast<short>(ceil(cell_pos.y()))};
    default: break;
    }
    return {};
}

auto Entity::wall_collision(Position const& pos, board_type const& actual_map, bool can_use_door) -> bool
{
    cen::fpoint const cell = pos.as_f();
    cen::fpoint const cell_pos = {cell.x() / BLOCK_SIZE_24, cell.y() / BLOCK_SIZE_24};
    for (unsigned char side_dir = 0; side_dir < 4; side_dir++) {
        Position board_pos = char_board_pos(side_dir, cell_pos);
        if (BlockType::Wall == actual_map[BOARD_WIDTH * board_pos.y() + abs(board_pos.x() % BOARD_WIDTH)]) {
            return true;
        } else if (BlockType::Door == actual_map[BOARD_WIDTH * board_pos.y() + abs(board_pos.x() % BOARD_WIDTH)]) {
            return !can_use_door;
        }
    }
    return false;
}

void Entity::move(Direction mover)
{
    using enum Direction;
    switch (mover) {
    case Right: position = position + Position{1,0}; break;
    case Up: position = position + Position{0,-1}; break;
    case Left: position = position + Position{-1,0}; break;
    case Down: position = position + Position{0,1}; break;
    default:
        break;
    }
}

void Entity::check_wrap()
{
    if (position.x() > WINDOW_WIDTH + BLOCK_SIZE_24)
        position.set_x(-BLOCK_SIZE_24);
    if (position.x() < -BLOCK_SIZE_24)
        position.set_x(WINDOW_WIDTH + BLOCK_SIZE_24);
}

