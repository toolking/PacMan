#include "Pinky.hpp"
#include "Entity.hpp"
#include "Ghost.hpp"
#include "Pac.hpp"
#include "Position.hpp"
#include "Globals.hpp"

Pinky::Pinky(cen::renderer_handle const& renderer)
  : Ghost(renderer,PINK, Entity::Type::Pinky)
{
    ScatterTarget = Position(2 * BLOCK_SIZE_24 + BLOCK_SIZE_24 / 2, BLOCK_SIZE_24 / 2);
    Home = Position(13 * BLOCK_SIZE_24 + BLOCK_SIZE_24 / 2, 17 * BLOCK_SIZE_24 + BLOCK_SIZE_24 / 2);
}

void Pinky::calculate_target(Entity pac)
{
    short const target_x = pac.position.x
                  + ((pac.direction() == Direction::Right) ?  4 * BLOCK_SIZE_24 :
                     (pac.direction() == Direction::Left)  ? -4 * BLOCK_SIZE_24 :
                                                              0);
    short const target_y = pac.position.y
                  + ((pac.direction() == Direction::Down) ?  4 * BLOCK_SIZE_24 :
                     (pac.direction() == Direction::Up)   ? -4 * BLOCK_SIZE_24 :
                                                             0);
    Target = {target_x, target_y};
}

void Pinky::update_pos(board_type const& actual_board, Pac const& pac, bool timed_status)
{
    update_speed(pac);
    update_status(pac, timed_status);
    for (unsigned char i = 0; i < speed(); i++) {
        update_facing(pac);
        if (is_target_to_calculate(pac)) {
            calculate_target(pac);
        }
        calculate_direction(actual_board);
        move(direction());
        check_wrap();
    }
}
