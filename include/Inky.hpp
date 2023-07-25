#pragma once

#include "Entity.hpp"
#include "Ghost.hpp"
#include "Pac.hpp"
#include "Position.hpp"

class Inky : public Ghost
{
public:
    Inky();
    void update_pos(Board::board_type const& actual_board, Pac const& pac, Position  const& blinky, bool timed_status);

private:
    void calculate_target(Entity pac, Position blinky);
};
