#pragma once

#include "Ghost.hpp"
#include "Pac.hpp"
#include "Position.hpp"

class Clyde : public Ghost
{
public:
    Clyde(cen::renderer_handle& renderer);
    void update_pos(board_type const& actual_board, Pac const& pac, Status timed_status);

private:
    void calculate_target(cen::ipoint const& pac);
};
