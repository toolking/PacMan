#include "Blinky.hpp"
#include "Entity.hpp"
#include "Ghost.hpp"
#include "Pac.hpp"
#include "Position.hpp"
#include "Globals.hpp"

Blinky::Blinky()
  : Ghost(RED, Entity::Type::Blinky)
{
    ScatterTarget = Position(25 * BOCK_SIZE_24 + BOCK_SIZE_24 / 2, BOCK_SIZE_24 / 2);
    Home = Position(13 * BOCK_SIZE_24 + BOCK_SIZE_24 / 2, 17 * BOCK_SIZE_24 + BOCK_SIZE_24 / 2);
}

void Blinky::calculate_target(Position pac)
{
    Target = pac;
}

void Blinky::update_pos(Board::board_type const& actual_board, Pac const& pac, bool timed_status)
{
    update_speed(pac);
    update_status(pac, timed_status);
    for (unsigned char i = 0; i < speed(); i++) {
        update_facing(pac);
        if (is_target_to_calculate(pac)) {
            calculate_target(pac.position);
        }
        calculate_direction(actual_board);
        move(direction());
        check_wrap();
    }
}
