#include "Pinky.hpp"
#include "Entity.hpp"
#include "Ghost.hpp"
#include "Pac.hpp"
#include "Position.hpp"
#include "Globals.hpp"

Pinky::Pinky(cen::renderer_handle& renderer)
  : Ghost(renderer,cen::colors::pink, Entity::Type::Pinky)
{
    ScatterTarget = coord_to_position(2,0);
    Home = coord_to_position(13,17);
}

void Pinky::calculate_target(Entity const& pac)
{
    // Pinky's target is 4 blocks ahead of Pacman
    constexpr auto dist = 4 * BLOCK_SIZE_24;
    Target = pac.position + direction_to_point(pac.direction) * dist;
}

void Pinky::update_pos(board_type const& actual_board, Pac const& pac, Status timed_status)
{
    update_speed(pac);
    update_status(pac, timed_status);
    for (unsigned char i = 0; i < speed; i++) {
        update_facing(pac);
        if (is_target_to_calculate(pac)) {
            calculate_target(pac);
        }
        calculate_direction(actual_board);
        move(direction);
        check_wrap();
    }
}
