#pragma once

#include "Ghost.hpp"
#include "Pac.hpp"
#include "Position.hpp"

class Blinky : public Ghost
{
public:
    Blinky(cen::renderer const& renderer);
    void update_pos(board_type const& actual_board, Pac const& pac, bool timed_status);

private:
    void calculate_target(Position pac);
};