#include "Inky.hpp"

Inky::Inky()
  : Ghost(CYAN, Entity::Type::Inky)
{
    ScatterTarget = Position(26 * BOCK_SIZE_24 + BOCK_SIZE_24 / 2, 35 * BOCK_SIZE_24 + BOCK_SIZE_24 / 2);
    Home = Position(11 * BOCK_SIZE_24 + BOCK_SIZE_24 / 2, 17 * BOCK_SIZE_24 + BOCK_SIZE_24 / 2);
}

void Inky::calculate_target(Entity pac, Position blinky)
{
    short x = pac.position().x;
    short y = pac.position().y;
    switch (pac.direction()) {
    case Direction::Right:
        x += 2 * BOCK_SIZE_24;
        break;
    case Direction::Up:
        y -= 2 * BOCK_SIZE_24;
        break;
    case Direction::Left:
        x -= 2 * BOCK_SIZE_24;
        break;
    case Direction::Down:
        y += 2 * BOCK_SIZE_24;
        break;
    default:
        break;
    }
    Target = Position(2*x - blinky.x, 2*y - blinky.y);
}

void Inky::update_pos(Board::board_type const& actual_board, Pac const& pac, Position blinky, bool timed_status)
{
    update_speed(pac);
    update_status(pac, timed_status);
    for (unsigned char i = 0; i < speed(); i++) {
        update_facing(pac);
        if (is_target_to_calculate(pac))
            calculate_target(pac, blinky);
        calculate_direction(actual_board);
        move(direction());
        check_wrap();
    }
}
