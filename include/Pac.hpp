#pragma once

#include "Entity.hpp"
#include "Texture.hpp"
#include "Globals.hpp"

#include <vector>
#include <array>

class Pac : public Entity
{
public:
    Pac(cen::renderer const& renderer)
      : Entity(Entity::Type::PacMan)
      , renderer_{renderer}
      , living_pac_{renderer_}
      , death_pac_{renderer_}
   {    living_pac_.load_from_file("Textures/PacMan32.png");
    death_pac_.load_from_file("Textures/GameOver32.png");
    }

    void update_pos(std::vector<Direction>& mover, board_type const& actual_map);
    unsigned char food_collision(board_type& actual_map);
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
    cen::renderer const& renderer_;
    Texture living_pac_;
    Texture death_pac_;
    std::array<SDL_Rect,LIVING_PAC_FRAMES> living_pac_sprite_clips_{init_frames<LIVING_PAC_FRAMES>()};
    std::array<SDL_Rect,DEATH_PAC_FRAMES> death_pac_sprite_clips_{init_frames<DEATH_PAC_FRAMES>()};
    unsigned char curr_living_pac_frame_ = 0 ;
    unsigned char curr_death_pac_frame_ = 0;
    bool energy_status_ = false;
    bool dead_animation_statement_ = false;
};
