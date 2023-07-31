#include "Fruit.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

Fruit::Fruit(cen::renderer_handle& renderer)
    : renderer_{renderer}
    , fruit_texture_{renderer_,"Textures/Fruit32.png"}
{
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

auto Fruit::is_eatable() -> bool
{
    return fruit_timer_.is_started();
}

auto Fruit::get_score_value() -> unsigned short
{
    return SCORE_TABLE[current_fruit_];
}

void Fruit::start_score_timer()
{
    score_timer_.start();
}

auto Fruit::check_despawn() -> bool
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
    using namespace std::chrono_literals;
    if (fruit_timer_.is_started()) {
        fruit_texture_.render(position.x() - 4, position.y() - 4, 0, current_fruit_);
    }
    if (score_timer_.is_started() && score_timer_.get_ticks() < 1s) {
        TextureFont<true> score_texture{renderer_, std::to_string(get_score_value())};
        score_texture.render(position.x(), position.y() - BLOCK_SIZE_24 / 2);
    }
}
