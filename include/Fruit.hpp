#pragma once

#include "Globals.hpp"
#include "Position.hpp"
#include "Texture.hpp"
#include "Timer.hpp"

#include <array>

class Fruit
{
public:
    Fruit(cen::renderer_handle& renderer);
    void mod_current_fruit(unsigned short actual_actual_level);
    void update_food_counter();
    bool is_eatable();
    unsigned short get_score_value();
    void start_score_timer();
    bool check_despawn();
    void despawn();
    void reset_food_counter();
    void draw();

    cen::ipoint const position {cen::ipoint {13, 20} * BLOCK_SIZE_24 + cen::ipoint {BLOCK_SIZE_24 / 2, 0}};

private:
    cen::renderer_handle& renderer_;
    TextureMultiImg<FRUIT_FRAMES> fruit_texture_;
    Timer fruit_timer_;
    Timer score_timer_;
    unsigned char current_fruit_ {0};
    cen::u64ms fruit_duration_ {9000};
    unsigned char food_counter_ {0};
};
