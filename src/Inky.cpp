#include "Inky.hpp"
#include "Entity.hpp"
#include "Ghost.hpp"
#include "Globals.hpp"
#include "Pac.hpp"
#include "Position.hpp"

Inky::Inky(cen::renderer_handle const& renderer)
  : Ghost(renderer, CYAN, Entity::Type::Inky)
{
    ScatterTarget = Position(26 * BLOCK_SIZE_24 + BLOCK_SIZE_24 / 2, 35 * BLOCK_SIZE_24 + BLOCK_SIZE_24 / 2);
    Home = Position(11 * BLOCK_SIZE_24 + BLOCK_SIZE_24 / 2, 17 * BLOCK_SIZE_24 + BLOCK_SIZE_24 / 2);
}

void Inky::calculate_target(Entity pac, Position pos_blinky)
{
    short const x = pac.position.x
                  + ((pac.direction() == Direction::Right) ?  2 * BLOCK_SIZE_24 :
                     (pac.direction() == Direction::Left)  ? -2 * BLOCK_SIZE_24 :
                                                              0);
    short const y = pac.position.y
                  + ((pac.direction() == Direction::Down) ?  2 * BLOCK_SIZE_24 :
                     (pac.direction() == Direction::Up)   ? -2 * BLOCK_SIZE_24 :
                                                             0);
    Target = Position(x + x - pos_blinky.x, y + y - pos_blinky.y);
}

void Inky::update_pos(board_type const& actual_board, Pac const& pac, Position const& pos_blinky, bool timed_status)
{
    update_speed(pac);
    update_status(pac, timed_status);
    for (unsigned char i = 0; i < speed(); i++) {
        update_facing(pac);
        if (is_target_to_calculate(pac)) {
            calculate_target(pac, pos_blinky);
        }
        calculate_direction(actual_board);
        move(direction());
        check_wrap();
    }
}
