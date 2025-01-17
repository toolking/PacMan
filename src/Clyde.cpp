#include "Clyde.hpp"
#include "Entity.hpp"
#include "Ghost.hpp"
#include "Globals.hpp"
#include "Pac.hpp"
#include "Position.hpp"

#include <cmath>

Clyde::Clyde(cen::renderer_handle& renderer)
  : Ghost(renderer, cen::colors::orange, Entity::Type::Clyde)
{
    ScatterTarget = coord_to_position(0, 35);
    Home = coord_to_position(15,17);
}

void Clyde::calculate_target(cen::ipoint const& pac_pos)
{
    auto const dist = wrapped_distance(position, pac_pos);
    Target = (dist > 8 * BLOCK_SIZE_24) ? pac_pos : ScatterTarget;
}

void Clyde::update_pos(board_type const& actual_board, Pac const& pac, Status timed_status)
{
    update_speed(pac);
    update_status(pac, timed_status);
    for (unsigned char i = 0; i < speed; i++) {
        update_facing(pac);
        if (is_target_to_calculate(pac)) {
            calculate_target(pac.position);
        }
        calculate_direction(actual_board);
        move(direction);
        check_wrap();
    }
}
