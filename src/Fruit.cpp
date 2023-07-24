#include "Fruit.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

Fruit::Fruit()
  : position_(13 * BOCK_SIZE_24 + BOCK_SIZE_24 / 2, 20 * BOCK_SIZE_24)
{
    fruit_texture_.load_from_file("Textures/Fruit32.png");
    init_frames(FRUIT_FRAMES, fruit_sprite_clips_);
}

Fruit::~Fruit()
{
    fruit_texture_.free();
}

void Fruit::mod_current_fruit(unsigned short actual_level)
{
    current_fruit_ = (actual_level > 21)?7:static_cast<unsigned char>(floor((actual_level - 1) / 3.0f));
}

void Fruit::update_food_counter()
{
    food_counter_++;
    if (food_counter_ == 70 || food_counter_ == 200)
        if (!fruit_timer_.is_started())
            fruit_timer_.start();
}

bool Fruit::is_eatable()
{
    return fruit_timer_.is_started();
}

unsigned short Fruit::get_score_value()
{
    return SCORE_TABLE[current_fruit_];
}

void Fruit::start_score_timer()
{
    score_timer_.start();
}

void Fruit::reset_score_timer()
{
    if (score_timer_.get_ticks() > 1000)
        score_timer_.reset();
}

bool Fruit::check_despawn()
{
    return fruit_timer_.get_ticks() > fruit_duration_;
}

void Fruit::despawn()
{
    fruit_timer_.reset();
}

void Fruit::reset_food_counter()
{
    food_counter_ = 0;
}

void Fruit::draw()
{
    if (fruit_timer_.is_started()) {
        auto clip = &fruit_sprite_clips_[current_fruit_];
        fruit_texture_.render(position().x - 4, position().y - 4, 0, clip);
    }
    if (score_timer_.is_started() && score_timer_.get_ticks() < 1000) {
        std::stringstream ss;
        ss << SCORE_TABLE[current_fruit_];
        Texture score_texture;
        score_texture.load_from_rendered_text(ss.str(), WHITE, 1);
        score_texture.render(position().x, position().y - BOCK_SIZE_24 / 2);
        score_texture.free();
    }
}
