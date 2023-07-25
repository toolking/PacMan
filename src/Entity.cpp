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
    case 0:
        return Position(static_cast<short>(floor(cell_x)), static_cast<short>(floor(cell_y)));
    case 1:
        return Position(static_cast<short>(ceil(cell_x)), static_cast<short>(floor(cell_y)));
    case 2:
        return Position(static_cast<short>(floor(cell_x)), static_cast<short>(ceil(cell_y)));
    case 3:
        return Position(static_cast<short>(ceil(cell_x)), static_cast<short>(ceil(cell_y)));
    default:
        break;
    }
    return Position{};
}

auto Entity::wall_collision(short x, short y, Board::board_type const& actual_map, bool can_use_door) -> bool
{
    float const cell_x = x / static_cast<float>(BOCK_SIZE_24);
    float const cell_y = y / static_cast<float>(BOCK_SIZE_24);
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
        ++position_.x;
        break;
    case Direction::Up:
        --position_.y;
        break;
    case Direction::Left:
        --position_.x;
        break;
    case Direction::Down:
        ++position_.y;
        break;
    default:
        break;
    }
}

void Entity::check_wrap()
{
    if (position_.x > WINDOW_WIDTH + BOCK_SIZE_24)
        position_.x = -BOCK_SIZE_24;
    if (position_.x < -BOCK_SIZE_24)
        position_.x = WINDOW_WIDTH + BOCK_SIZE_24;
}

