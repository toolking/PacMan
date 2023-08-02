#include "Entity.hpp"
#include "Globals.hpp"

void Entity::move(Direction mover)
{
    position = position_in_direction(position, mover);
}

void Entity::check_wrap()
{
    if (position.x() > WINDOW_WIDTH + BLOCK_SIZE_24)
        position.set_x(-BLOCK_SIZE_24);
    else if (position.x() < -BLOCK_SIZE_24)
        position.set_x(WINDOW_WIDTH + BLOCK_SIZE_24);
}
