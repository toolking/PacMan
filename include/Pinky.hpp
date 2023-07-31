#pragma once

#include "Entity.hpp"
#include "Ghost.hpp"
#include "Pac.hpp"

class Pinky : public Ghost
{
public:
    Pinky(cen::renderer_handle& renderer);
    void update_pos(board_type const& actual_board, Pac const& pac, Status timed_status);

private:
    void calculate_target(Entity const& pac);
};
