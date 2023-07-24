#pragma once

#include "Entity.hpp"
#include "Pac.hpp"
#include "Texture.hpp"
#include "Timer.hpp"

#include <SDL2/SDL.h>
#include <vector>

class Ghost : public Entity
{
public:
    Ghost(SDL_Color color, EntityType identity);
    ~Ghost();
    bool is_target_to_calculate(Pac const& pac);
    void poss_dirs_bubble_sort(std::vector<float>& distances, std::vector<Direction>& possible_directions);
    void calculate_direction(Board::board_type const& actual_map);
    bool is_home();
    void mod_status(bool status);
    void update_status(Pac const& pac, bool timed_status);
    void update_facing(Pac const& pac);
    void update_speed(Pac const& pac);
    void draw(Pac const& pac, Timer timer, unsigned short timer_target);

protected:
    Position Target;
    Position ScatterTarget;
    Position DoorTarget;
    Position Home;

private:
    Texture body_;
    Texture eyes_;
    SDL_Rect ghost_body_sprite_clips_[GHOST_BODY_FRAMES];
    SDL_Rect ghost_eye_sprite_clips_[GHOST_EYE_FRAMES];
    SDL_Color color_;
    unsigned char current_body_frame_;
    bool can_use_door_;
    bool status_; // false -> chase	true -> scatter
};