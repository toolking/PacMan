#include "Entity.hpp"
#include "Globals.hpp"

void Entity::get_possible_position(short& x, short& y, Direction mover)
{
    switch (mover) {
    case Direction::Right:
        x++;
        break;
    case Direction::Up:
        y--;
        break;
    case Direction::Left:
        x--;
        break;
    case Direction::Down:
        y++;
        break;
    default:
        break;
    }
}

auto Entity::char_board_pos(unsigned char side_dir, float cell_x, float cell_y) -> Position
{
    switch (side_dir) {
    case 0: return {static_cast<short>(floor(cell_x)), static_cast<short>(floor(cell_y))};
    case 1: return {static_cast<short>(ceil(cell_x)), static_cast<short>(floor(cell_y))};
    case 2: return {static_cast<short>(floor(cell_x)), static_cast<short>(ceil(cell_y))};
    case 3: return {static_cast<short>(ceil(cell_x)), static_cast<short>(ceil(cell_y))};
    default: break;
    }
    return {};
}

auto Entity::wall_collision(short x, short y, board_type const& actual_map, bool can_use_door) -> bool
{
    float const cell_x = x / static_cast<float>(BLOCK_SIZE_24);
    float const cell_y = y / static_cast<float>(BLOCK_SIZE_24);
    for (unsigned char side_dir = 0; side_dir < 4; side_dir++) {
        Position board_pos = char_board_pos(side_dir, cell_x, cell_y);
        if (BlockType::Wall == actual_map[BOARD_WIDTH * board_pos.y + abs(board_pos.x % BOARD_WIDTH)]) {
            return true;
        } else if (BlockType::Door == actual_map[BOARD_WIDTH * board_pos.y + abs(board_pos.x % BOARD_WIDTH)]) {
            return !can_use_door;
        }
    }
    return false;
}

void Entity::move(Direction mover)
{
    switch (mover) {
    case Direction::Right:
        ++position.x;
        break;
    case Direction::Up:
        --position.y;
        break;
    case Direction::Left:
        --position.x;
        break;
    case Direction::Down:
        ++position.y;
        break;
    default:
        break;
    }
}

void Entity::check_wrap()
{
    if (position.x > WINDOW_WIDTH + BLOCK_SIZE_24)
        position.x = -BLOCK_SIZE_24;
    if (position.x < -BLOCK_SIZE_24)
        position.x = WINDOW_WIDTH + BLOCK_SIZE_24;
}

