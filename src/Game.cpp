#include "Game.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

void Game::reset_ghosts_life_statement()
{
    blinky_.life_statement(true);
    inky_.life_statement(true);
    pinky_.life_statement(true);
    clyde_.life_statement(true);
}

void Game::reset_ghosts_facing()
{
    blinky_.facing(0);
    inky_.facing(1);
    pinky_.facing(1);
    clyde_.facing(1);
}

void Game::start()
{
    if (is_game_started_) {
        ready_.render(11 * BLOCK_SIZE_24, 20 * BLOCK_SIZE_24 - 5);
        return;
    }
    if (is_level_completed()) {
        actual_map_=board_.map();
    }
    pac_.position=board_.entity_start_position(pac_);
    blinky_.position=board_.entity_start_position(blinky_);
    inky_.position=board_.entity_start_position(inky_);
    pinky_.position=board_.entity_start_position(pinky_);
    clyde_.position=board_.entity_start_position(clyde_);
    pac_.change_energy_status(false);
    reset_ghosts_life_statement();
    reset_ghosts_facing();
    pac_.reset_current_living_frame();
    ghost_timer_.restart();
    is_game_started_ = true;
    ghost_timer_.start();

    ready_.render(11 * BLOCK_SIZE_24, 20 * BLOCK_SIZE_24 - 5);
}

void Game::mod_start_statement(bool new_start_statement)
{
    is_game_started_ = new_start_statement;
}

void Game::clock()
{
    if (ghost_timer_.get_ticks() <= ghost_timer_target_)
        return;
    if (ghost_timer_target_ == scatter_time_) {
        if (pac_.is_energized()) {
            pac_.change_energy_status(false);
        }
        timed_status_ = false;
        ghost_timer_target_ = chasing_time_;
        ghost_timer_.restart();
    } else if (ghost_timer_target_ == chasing_time_) {
        timed_status_ = true;
        ghost_timer_target_ = scatter_time_;
        ghost_timer_.restart();
    }
}

void Game::update_positions(std::vector<Direction>& mover, bool timed_status)
{
    blinky_.update_pos(actual_map_, pac_, timed_status);
    inky_.update_pos(actual_map_, pac_, blinky_.position, timed_status);
    pinky_.update_pos(actual_map_, pac_, timed_status);
    clyde_.update_pos(actual_map_, pac_, timed_status);
    pac_.update_pos(mover, actual_map_);
}

void Game::food()
{
    switch (pac_.food_collision(actual_map_)) {
    case 0:
        board_.score_increase(0);
        fruit_.update_food_counter();
        if (is_to_waka_sound_) {
            sound.play_waka();
            is_to_waka_sound_ = false;
        }
        waka_timer_.reset();
        break;
    case 1:
        board_.score_increase(1);
        fruit_.update_food_counter();
        pac_.change_energy_status(true);
        scorer_ = 200;
        ghost_timer_target_ = scatter_time_;
        ghost_timer_.restart();
        if (is_to_waka_sound_) {
            sound.play_waka();
            is_to_waka_sound_ = false;
        }
        waka_timer_.reset();
        break;
    default:
        if (!waka_timer_.is_started())
            waka_timer_.start();
        break;
    }
    if (waka_timer_.get_ticks() > 300) {
        sound.stop_waka();
        is_to_waka_sound_ = true;
    }
}

void Game::entity_collisions()
{
    if (!pac_.is_energized()) {
        deadly_pac_ghost_coll();
        if (!is_to_scatter_sound_) {
            dead_ghosts_counter_ = 0;
            sound.stop_scatter_ghost();
            is_to_scatter_sound_ = true;
        }
    } else {
        if (is_to_scatter_sound_) {
            sound.play_scatter_ghost();
            is_to_scatter_sound_ = false;
        }
        deadly_ghost_pac_coll(blinky_);
        deadly_ghost_pac_coll(inky_);
        deadly_ghost_pac_coll(pinky_);
        deadly_ghost_pac_coll(clyde_);
        if (dead_ghosts_counter_ == 4) {
            if (!is_to_scatter_sound_) {
                sound.stop_scatter_ghost();
            }
        }
    }
    if (fruit_.is_eatable()) {
        if (is_colliding(pac_.position,fruit_.position)) {
            fruit_.start_score_timer();
            board_.score_increase(fruit_.get_score_value());
            fruit_.despawn();
            sound.play_eat_fruit();
        } else {
            if (fruit_.check_despawn())
                fruit_.despawn();
        }
    }
}

void Game::update(std::vector<Direction>& mover)
{
    clock();
    update_positions(mover, timed_status_);
    food();
    if (board_.is_extra_life())
        sound.play_extra_life();
    entity_collisions();
}

auto Game::get_level() -> unsigned short
{
    return level_;
}

void Game::increase_level()
{
    level_++;
}

void Game::update_difficulty()
{
    if (level_ % 3 == 0) {
        chasing_time_ += 1000;
        if (scatter_time_ > 2000) {
            scatter_time_ -= 1000;
        }
    }
}

auto Game::is_level_completed() -> bool
{
    return std::none_of(actual_map_.cbegin(),actual_map_.cend(),[](BlockType b){
        return b == BlockType::Pellet || b == BlockType::Energizer;
    });
}

void Game::clear_mover(std::vector<Direction>& mover)
{
    mover.clear();
    mover.emplace_back(Direction::Nowhere);
}

void Game::deadly_pac_ghost_coll()
{
    if (   (is_colliding(pac_.position,blinky_.position) && blinky_.is_alive())
        || (is_colliding(pac_.position,inky_.position) && inky_.is_alive())
        || (is_colliding(pac_.position,pinky_.position) && pinky_.is_alive())
        || (is_colliding(pac_.position,clyde_.position) && clyde_.is_alive()))
        pac_.life_statement(false);
}

void Game::deadly_ghost_pac_coll(Ghost& ghost)
{
    if (is_colliding(pac_.position,ghost.position) && ghost.is_alive()) {
        ghost.life_statement(false);
        board_.score_increase(scorer_);
        little_score_scorers_.push_back(scorer_);
        Timer ghost_lil_timer;
        ghost_lil_timer.start();
        little_score_timers_.push_back(ghost_lil_timer);
        Position this_lil_pos;
        this_lil_pos = ghost.position;
        little_score_positions_.push_back(this_lil_pos);
        scorer_ *= 2;
        sound.play_ghost_death();
        dead_ghosts_counter_++;
    }
}

void Game::mod_to_waka(bool new_waka)
{
    is_to_waka_sound_ = new_waka;
}

void Game::death_sound()
{
    if (!is_to_death_pac_sound_) {
        return;
    }
    sound.stop_waka();
    sound.play_pac_death();
    is_to_death_pac_sound_ = false;
}

void Game::mod_death_sound_statement(bool new_death_sound_statement)
{
    is_to_death_pac_sound_ = new_death_sound_statement;
}

void Game::draw_little_score()
{
    for (unsigned char i = 0; i < little_score_timers_.size(); i++) {
        Timer this_lil_timer = little_score_timers_.at(i);
        if (this_lil_timer.get_ticks() < little_timer_target_) {
            std::stringstream ss;
            ss << little_score_scorers_.at(i);
            TextureFont<true> this_lil_texture{renderer_,ss.str(), WHITE};
            Position this_lil_pos = little_score_positions_.at(i);
            this_lil_texture.render(this_lil_pos.x, this_lil_pos.y - BLOCK_SIZE_24 / 2);
        } else {
            little_score_scorers_.erase(little_score_scorers_.begin() + i);
            little_score_timers_.erase(little_score_timers_.begin() + i);
            little_score_positions_.erase(little_score_positions_.begin() + i);
        }
    }
}

auto Game::process(Timer& game_timer, std::vector<Direction>& mover, unsigned short& start_ticks) -> bool
{
    // Returns false when should render the last animation frame.
    // It's bad looking, so I don't want to render it.
    if (game_timer.get_ticks() < start_ticks) {
        start();
        return true;
    }
    if (pac_.is_alive()) {
        if (!is_level_completed()) {
            update(mover);
        } else {
            if (!map_animation_timer_.is_started()) {
                if (start_ticks != 2500)
                    start_ticks = 2500;
                pac_.reset_current_living_frame();
                fruit_.despawn();
                fruit_.reset_food_counter();
                sound.stop_waka();
                sound.stop_scatter_ghost();
                mod_to_waka(true);
                map_animation_timer_.start();
            } else if (map_animation_timer_.get_ticks() > 2100) {
                clear_mover(mover);
                increase_level();
                fruit_.mod_current_fruit(get_level());
                update_difficulty();
                mod_start_statement(false);
                map_animation_timer_.reset();
                game_timer.start();
                return false;
            }
        }
    } else {
        if (board_.get_lives() > 0) {
            if (pac_.is_dead_animation_ended()) {
                if (start_ticks != 2500)
                    start_ticks = 2500;
                clear_mover(mover);
                pac_.mod_dead_animation_statement(false);
                pac_.life_statement(true);
                board_.decrease_lives();
                fruit_.despawn();
                mod_to_waka(true);
                mod_death_sound_statement(true);
                mod_start_statement(false);
                game_timer.restart();
                return false;
            }
        } else {
            if (pac_.is_dead_animation_ended()) {
                mod_start_statement(false);
            }
        }
        death_sound();
    }

    return true;
}

void Game::draw()
{
    board_.set_high_score();
    board_.set_score();
    board_.draw(actual_map_, map_animation_timer_);
    if (!is_game_started_) {
        game_over_texture_.render(9 * BLOCK_SIZE_24, 20 * BLOCK_SIZE_24 - 5);
        return;
    }
    fruit_.draw();
    if (!map_animation_timer_.is_started()) {
        clyde_.draw(pac_, ghost_timer_, scatter_time_);
        pinky_.draw(pac_, ghost_timer_, scatter_time_);
        inky_.draw(pac_, ghost_timer_, scatter_time_);
        blinky_.draw(pac_, ghost_timer_, scatter_time_);
        draw_little_score();
    }
    pac_.draw();
}
