#include "Pinky.hpp"

Pinky::Pinky()
  : Ghost(PINK, Entity::Type::Pinky)
{
    ScatterTarget = Position(2 * BOCK_SIZE_24 + BOCK_SIZE_24 / 2, BOCK_SIZE_24 / 2);
    Home = Position(13 * BOCK_SIZE_24 + BOCK_SIZE_24 / 2, 17 * BOCK_SIZE_24 + BOCK_SIZE_24 / 2);
}

void Pinky::calculate_target(Entity pac)
{
    short x = pac.position().x;
    short y = pac.position().y;
    switch (pac.direction()) {
    case Direction::Right:
        x += 4 * BOCK_SIZE_24;
        break;
    case Direction::Up:
        y -= 4 * BOCK_SIZE_24;
        break;
    case Direction::Left:
        x -= 4 * BOCK_SIZE_24;
        break;
    case Direction::Down:
        y += 4 * BOCK_SIZE_24;
        break;
    default:
        break;
    }
    Target = Position(x, y);
}

void Pinky::update_pos(Board::board_type const& actual_board, Pac const& pac, bool timed_status)
{
    update_speed(pac);
    update_status(pac, timed_status);
    for (unsigned char i = 0; i < speed(); i++) {
        update_facing(pac);
        if (is_target_to_calculate(pac))
            calculate_target(pac);
        calculate_direction(actual_board);
        move(direction());
        check_wrap();
    }
}
