#pragma once

#include "Ghost.hpp"
#include "Pac.hpp"
#include "Position.hpp"

class Clyde : public Ghost
{
public:
    Clyde();
    void calculate_target(Position pac);
    void update_pos(Board::board_type const& actual_board, Pac const& pac, bool timed_status);
};
