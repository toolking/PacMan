#pragma once

#include "Entity.hpp"
#include "Ghost.hpp"
#include "Pac.hpp"
#include "Position.hpp"

class Inky : public Ghost
{
public:
    Inky(cen::renderer_handle& renderer);
    void update_pos(board_type const& actual_board, Pac const& pac, cen::ipoint  const& blinky, Status timed_status);

private:
    void calculate_target(Entity pac, cen::ipoint blinky);
};
