#include "Board.hpp"

#include "Entity.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
void Board::set_score()
{
    score_texture_.set_new_text(std::to_string(score_));
    if (score_ > high_score_) {
        high_score_ = score_;
        std::ofstream high_score_file("HighScore.txt");
        high_score_file << high_score_;
        high_score_texture_.set_new_text(std::to_string(high_score_));
        high_score_file.close();
    }
}

void Board::draw(board_type const& actual_map, Timer map_animation_timer)
{
    map_texture_.render(0, 0);
    score_label_texture_.render(0, 0);
    score_texture_.render(0, BLOCK_SIZE_32);
    high_score_label_texture_.render(WINDOW_WIDTH / 2, 0);
    high_score_texture_.render(WINDOW_WIDTH / 2, BLOCK_SIZE_32);
    for (unsigned char i = 1; i <= lives_; i++) {
        lives_texture_.render(i * BLOCK_SIZE_32, 26 * BLOCK_SIZE_32 - BLOCK_SIZE_32 / 4);
    }
    if (map_animation_timer.is_started()) {
        using namespace std::chrono_literals;
        auto const color {
            ((map_animation_timer.get_ticks() / 250) % 2 == 1ms)
                ? cen::colors::white
                : cen::colors::blue};
        map_texture_.set_color(color);
        return;
    }
    door_texture_.render(WINDOW_WIDTH / 2 - 23, WINDOW_HEIGHT / 2 - 57);
    unsigned short i = 0;
    for (auto const& c : actual_map) {
        if (c == BlockType::Pellet)
            pellet_texture_.render(i % BOARD_WIDTH * BLOCK_SIZE_24, i / BOARD_WIDTH * BLOCK_SIZE_24);
        else if (c == BlockType::Energizer)
            energizer_texture_.render(i % BOARD_WIDTH * BLOCK_SIZE_24, i / BOARD_WIDTH * BLOCK_SIZE_24);
        ++i;
    }
}

auto Board::is_extra_life() -> bool
{
    if (is_extra_ || score_ < 10000) {
        return false;
    }
    is_extra_ = true;
    lives_++;
    return true;
}

void Board::decrease_lives()
{
    lives_--;
}

auto Board::get_lives() -> unsigned char
{
    return lives_;
}

void Board::score_increase(unsigned short scorer)
{
    switch (scorer) {
    case 0: score_ += 10; break;
    case 1: score_ += 50; break;
    default: score_ += scorer; break;
    }
}
