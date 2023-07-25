#pragma once

#include "Entity.hpp"
#include "Ghost.hpp"
#include "Pac.hpp"

class Pinky : public Ghost
{
public:
    Pinky();
    void update_pos(Board::board_type const& actual_board, Pac const& pac, bool timed_status);

private:
    void calculate_target(Entity pac);
};
