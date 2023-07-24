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

void Entity::char_board_pos(unsigned char side_dir, Position& board_pos, float cell_x, float cell_y)
{
    switch (side_dir) {
    case 0:
        board_pos.x = static_cast<short>(floor(cell_x));
        board_pos.y = static_cast<short>(floor(cell_y));
        break;
    case 1:
        board_pos.x = static_cast<short>(ceil(cell_x));
        board_pos.y = static_cast<short>(floor(cell_y));
        break;
    case 2:
        board_pos.x = static_cast<short>(floor(cell_x));
        board_pos.y = static_cast<short>(ceil(cell_y));
        break;
    case 3:
        board_pos.x = static_cast<short>(ceil(cell_x));
        board_pos.y = static_cast<short>(ceil(cell_y));
        break;
    default:
        break;
    }
}

auto Entity::wall_collision(short x, short y, Board::board_type const& actual_map, bool can_use_door) -> bool
{
    float cell_x = x / static_cast<float>(BOCK_SIZE_24);
    float cell_y = y / static_cast<float>(BOCK_SIZE_24);
    Position BoardPos;
    for (unsigned char SideDir = 0; SideDir < 4; SideDir++) {
        char_board_pos(SideDir, BoardPos, cell_x, cell_y);
        if (BlockType::Wall == actual_map[BOARD_WIDTH * BoardPos.y + abs(BoardPos.x % BOARD_WIDTH)]) {
            return true;
        } else if (BlockType::Door == actual_map[BOARD_WIDTH * BoardPos.y + abs(BoardPos.x % BOARD_WIDTH)]) {
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

