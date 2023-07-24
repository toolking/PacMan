#pragma once

#include "Ghost.hpp"
#include "Pac.hpp"
#include "Position.hpp"

class Blinky : public Ghost
{
public:
    Blinky();
    void calculate_target(Position pac);
    void update_pos(Board::board_type const& actual_board, Pac const& pac, bool timed_status);
};