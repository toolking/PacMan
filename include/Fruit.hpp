#pragma once

#include "Globals.hpp"
#include "Position.hpp"
#include "Texture.hpp"
#include "Timer.hpp"

#include <SDL2/SDL.h>

class Fruit
{
public:
    Fruit();
    ~Fruit();
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
    void position(Position const& position)
    {
        position_ = position;
    }
    Position position() const
    {
        return position_;
    }
private:
    Texture fruit_texture_;
    Timer fruit_timer_;
    Timer score_timer_;
    SDL_Rect fruit_sprite_clips_[FRUIT_FRAMES];
    unsigned char current_fruit_ {0};
    unsigned short fruit_duration_ {9000};
    unsigned char food_counter_ {0};
    Position position_ {0, 0};
};
