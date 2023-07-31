#include "Entity.hpp"
#include "Globals.hpp"

auto Entity::get_possible_position(cen::ipoint const& position, Direction mover) -> cen::ipoint
{
    using enum Direction;
    switch (mover) {
    case Right: return position + cen::ipoint{1, 0};
    case Up: return position + cen::ipoint{0, -1};
    case Left: return position + cen::ipoint{-1, 0};
    case Down: return position + cen::ipoint{0, 1};
    default: return position;
    }
}

auto Entity::char_board_pos(unsigned char side_dir, cen::fpoint const& cell_pos) -> cen::ipoint
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

auto Entity::wall_collision(cen::ipoint const& pos, board_type const& actual_map, bool can_use_door) -> bool
{
    cen::fpoint const cell = pos.as_f();
    cen::fpoint const cell_pos = {cell.x() / BLOCK_SIZE_24, cell.y() / BLOCK_SIZE_24};
    for (unsigned char side_dir = 0; side_dir < 4; side_dir++) {
        cen::ipoint board_pos = char_board_pos(side_dir, cell_pos);
        auto const current_block_type = actual_map[BOARD_WIDTH * board_pos.y() + abs(board_pos.x() % BOARD_WIDTH)];
        if (BlockType::Wall == current_block_type) {
            return true;
        } else if (BlockType::Door == current_block_type) {
            return !can_use_door;
        }
    }
    return false;
}

void Entity::move(Direction mover)
{
    using enum Direction;
    switch (mover) {
    case Right: position = position + cen::ipoint{1,0}; break;
    case Up: position = position + cen::ipoint{0,-1}; break;
    case Left: position = position + cen::ipoint{-1,0}; break;
    case Down: position = position + cen::ipoint{0,1}; break;
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

