#include "Inky.hpp"
#include "Entity.hpp"
#include "Ghost.hpp"
#include "Globals.hpp"
#include "Pac.hpp"
#include "Position.hpp"

Inky::Inky(cen::renderer_handle& renderer)
  : Ghost(renderer, cen::colors::cyan, Entity::Type::Inky)
{
    ScatterTarget = coord_to_position(26,35);
    Home = coord_to_position(11,17);
}

void Inky::calculate_target(Entity pac, cen::ipoint pos_blinky)
{
    auto const x = pac.position.x()
                  + ((pac.direction == Direction::Right) ?  2 * BLOCK_SIZE_24 :
                     (pac.direction == Direction::Left)  ? -2 * BLOCK_SIZE_24 :
                                                              0);
    auto const y = pac.position.y()
                  + ((pac.direction == Direction::Down) ?  2 * BLOCK_SIZE_24 :
                     (pac.direction == Direction::Up)   ? -2 * BLOCK_SIZE_24 :
                                                             0);
    Target = cen::ipoint(x + x - pos_blinky.x(), y + y - pos_blinky.y());
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
