#pragma once

#include "Entity.hpp"
#include "Globals.hpp"
#include "Texture.hpp"

#include <array>
#include <vector>

class Pac : public Entity
{
public:
    Pac(cen::renderer_handle& renderer)
      : Entity(Entity::Type::PacMan)
      , renderer_ {renderer}
      , living_pac_ {renderer_, "Textures/PacMan32.png"}
      , death_pac_ {renderer_, "Textures/GameOver32.png"}
    {
    }

    void update_pos(std::vector<Direction>& mover, board_type const& actual_map);
    BlockType food_collision(board_type& actual_map);
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
    cen::renderer_handle& renderer_;
    TextureMultiImg<LIVING_PAC_FRAMES> living_pac_;
    TextureMultiImg<DEATH_PAC_FRAMES> death_pac_;
    unsigned char curr_living_pac_frame_ = 0;
    unsigned char curr_death_pac_frame_ = 0;
    bool energy_status_ = false;
    bool dead_animation_statement_ = false;
};
