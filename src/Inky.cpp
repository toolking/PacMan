#include "Inky.hpp"
#include "Entity.hpp"
#include "Ghost.hpp"
#include "Globals.hpp"
#include "Pac.hpp"
#include "Position.hpp"

Inky::Inky(cen::renderer_handle& renderer)
  : Ghost(renderer, cen::colors::cyan, Entity::Type::Inky)
{
    ScatterTarget = coord_to_position(26, 35);
    Home = coord_to_position(11, 17);
}

void Inky::calculate_target(Entity const& pac, cen::ipoint const& pos_blinky)
{
    using enum Direction;
    // Inky's target is twice the distance between Pac and Blinky in the direction of Pac
    constexpr auto dist = 2 * BLOCK_SIZE_24;
    switch (pac.direction) {
    case Right: Target = pac.position + cen::ipoint {dist, 0}; break;
    case Up: Target = pac.position + cen::ipoint {0, -dist}; break;
    case Left: Target = pac.position + cen::ipoint {-dist, 0}; break;
    case Down: Target = pac.position + cen::ipoint {0, dist}; break;
    default: Target = pac.position;
    }
    Target = Target + Target - pos_blinky;
}

void Inky::update_pos(board_type const& actual_board, Pac const& pac, cen::ipoint const& pos_blinky, Status timed_status)
{
    update_speed(pac);
    update_status(pac, timed_status);
    for (unsigned char i = 0; i < speed; i++) {
        update_facing(pac);
        if (is_target_to_calculate(pac)) {
            calculate_target(pac, pos_blinky);
        }
        calculate_direction(actual_board);
        move(direction);
        check_wrap();
    }
}
