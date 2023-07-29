#pragma once

#include "Globals.hpp"
#include "Position.hpp"
#include "Texture.hpp"
#include "Timer.hpp"

#include <array>

class Fruit
{
public:
    Fruit(cen::renderer_handle const& renderer);
    void mod_current_fruit(unsigned short actual_actual_level);
    void update_food_counter();
    bool is_eatable();
    unsigned short get_score_value();
    void start_score_timer();
    void reset_score_timer();
    bool check_despawn();
    void despawn();
    void reset_food_counter();
    void draw();

    Position position {13 * BLOCK_SIZE_24 + BLOCK_SIZE_24 / 2, 20 * BLOCK_SIZE_24};

private:
    cen::renderer_handle renderer_;
    TextureImg fruit_texture_;
    Timer fruit_timer_;
    Timer score_timer_;
    std::array<cen::irect,FRUIT_FRAMES> fruit_sprite_clips_{init_frames<FRUIT_FRAMES>()};
    unsigned char current_fruit_ {0};
    cen::u64ms fruit_duration_ {9000};
    unsigned char food_counter_ {0};
};
