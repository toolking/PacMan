#pragma once

#include "Entity.hpp"
#include "Pac.hpp"
#include "Texture.hpp"
#include "Timer.hpp"

#include <vector>
#include <array>

class Ghost : public Entity
{
public:
    Ghost(cen::renderer_handle& renderer, cen::color color, Entity::Type identity);
    bool is_target_to_calculate(Pac const& pac);
    void poss_dirs_bubble_sort(std::vector<float>& distances, std::vector<Direction>& possible_directions);
    void calculate_direction(board_type const& actual_map);
    bool is_home();
    void mod_status(bool status);
    void update_status(Pac const& pac, bool timed_status);
    void update_facing(Pac const& pac);
    void update_speed(Pac const& pac);
    void draw(Pac const& pac, Timer timer, cen::u64ms timer_target);

protected:
    Position Target;
    Position ScatterTarget;
    Position DoorTarget = Position(13 * BLOCK_SIZE_24 + BLOCK_SIZE_24 / 2, 15 * BLOCK_SIZE_24);
    Position Home;

private:
    cen::renderer_handle& renderer_;
    TextureImg body_;
    TextureImg eyes_;
    std::array<cen::irect,GHOST_BODY_FRAMES> ghost_body_sprite_clips_{init_frames<GHOST_BODY_FRAMES>()};
    std::array<cen::irect,GHOST_EYE_FRAMES> ghost_eye_sprite_clips_{init_frames<GHOST_EYE_FRAMES>()};
    cen::color color_;
    unsigned char current_body_frame_ = 0;
    bool can_use_door_ = false;
    bool status_ = false; // false -> chase	true -> scatter
};
