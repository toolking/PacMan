#pragma once

#include "Entity.hpp"
#include "Texture.hpp"
#include "Globals.hpp"

#include <vector>

class Pac : public Entity
{
public:
    Pac();
    ~Pac();
    void update_pos(std::vector<Direction>& mover, Board::board_type const& actual_map);
    unsigned char food_collision(Board::board_type& actual_map);
    bool is_energized() const;
    void change_energy_status(bool energy_status);
    void set_facing(Direction mover);
    bool is_dead_animation_ended();
    void mod_dead_animation_statement(bool new_dead_animation_statement);
    void update_current_living_pac_frame();
    void reset_current_living_frame();
    void wall_collision_frame();
    void draw();

private:
    Texture living_pac_;
    Texture death_pac_;
    SDL_Rect living_pac_sprite_clips_[LIVING_PAC_FRAMES];
    SDL_Rect death_pac_sprite_clips_[DEATH_PAC_FRAMES];
    unsigned char curr_living_pac_frame_;
    unsigned char curr_death_pac_frame_;
    bool energy_status_;
    bool dead_animation_statement_;
};
