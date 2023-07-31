#include "Game.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

void Game::reset_ghosts()
{
    blinky_.reset();
    inky_.reset();
    pinky_.reset();
    clyde_.reset();
    blinky_.facing = 0;
    inky_.facing = 1;
    pinky_.facing = 1;
    clyde_.facing = 1;
}

void Game::start()
{
    if (is_game_started_) {
        ready_.render(11 * BLOCK_SIZE_24, 20 * BLOCK_SIZE_24 - 5);
        return;
    }
    if (is_level_completed()) {
        actual_map_ = Board::BIN_BOARD;
    }
    pac_.reset();
    pac_.change_energy_status(false);
    reset_ghosts();
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
        timed_status_ = Ghost::Status::Chase;
        ghost_timer_target_ = chasing_time_;
        ghost_timer_.restart();
    } else if (ghost_timer_target_ == chasing_time_) {
        timed_status_ = Ghost::Status::Scatter;
        ghost_timer_target_ = scatter_time_;
        ghost_timer_.restart();
    }
}

void Game::update_positions(std::vector<Direction>& mover, Ghost::Status timed_status)
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
    using namespace std::chrono_literals;
    if (waka_timer_.get_ticks() > 300ms) {
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
        if (is_colliding(pac_.position, fruit_.position)) {
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
    using namespace std::chrono_literals;
    if (level_ % 3 == 0) {
        chasing_time_ += 1000ms;
        if (scatter_time_ > 2000ms) {
            scatter_time_ -= 1000ms;
        }
    }
}

auto Game::is_level_completed() -> bool
{
    using enum BlockType;
    return std::none_of(actual_map_.cbegin(), actual_map_.cend(), [](BlockType b) {
        return b == Pellet || b == Energizer;
    });
}

void Game::clear_mover(std::vector<Direction>& mover)
{
    mover.clear();
    mover.emplace_back(Direction::Nowhere);
}

void Game::deadly_pac_ghost_coll()
{
    if ((is_colliding(pac_.position, blinky_.position) && blinky_.is_alive)
        || (is_colliding(pac_.position, inky_.position) && inky_.is_alive)
        || (is_colliding(pac_.position, pinky_.position) && pinky_.is_alive)
        || (is_colliding(pac_.position, clyde_.position) && clyde_.is_alive))
        pac_.is_alive = false;
}

void Game::deadly_ghost_pac_coll(Ghost& ghost)
{
    if (is_colliding(pac_.position, ghost.position) && ghost.is_alive) {
        ghost.is_alive = false;
        board_.score_increase(scorer_);
        Timer little_score_timer;
        little_score_timer.start();
        little_score_entries_.emplace_back(little_score_timer, ghost.position, scorer_);
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
    little_score_entries_.remove_if([&](auto const& i) {
        return std::get<Timer>(i).get_ticks() >= little_timer_target_;
    });
    for (auto& i : little_score_entries_) {
        auto const score {std::get<unsigned short>(i)};
        auto const& position {std::get<cen::ipoint>(i)};
        TextureFont<true> this_lil_texture {renderer_, std::to_string(score)};
        this_lil_texture.render(position.x(), position.y() - BLOCK_SIZE_24 / 2);
    }
}

auto Game::process(Timer& game_timer, std::vector<Direction>& mover, cen::u64ms& start_ticks) -> bool
{
    using namespace std::chrono_literals;
    // Returns false when should render the last animation frame.
    // It's bad looking, so I don't want to render it.
    if (game_timer.get_ticks() < start_ticks) {
        start();
        return true;
    }
    if (pac_.is_alive) {
        if (!is_level_completed()) {
            update(mover);
        } else {
            if (!map_animation_timer_.is_started()) {
                start_ticks = 2500ms;
                pac_.reset_current_living_frame();
                fruit_.despawn();
                fruit_.reset_food_counter();
                sound.stop_waka();
                sound.stop_scatter_ghost();
                mod_to_waka(true);
                map_animation_timer_.start();
            } else if (map_animation_timer_.get_ticks() > 2100ms) {
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
                start_ticks = 2500ms;
                clear_mover(mover);
                pac_.mod_dead_animation_statement(false);
                pac_.is_alive = true;
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

void Game::run()
{
    Timer game_timer;
    cen::u64ms start_ticks {START_WAIT_TICKS};
    std::vector<Direction> mover;
    mover.push_back(Direction::Nowhere);
    game_timer.start();
    sound.play_intro();

    cen::event_handler handler;
    bool running = true;
    while (running) {
        auto const iteration_start {cen::now()};
        while (handler.poll()) {
            if (handler.is<cen::quit_event>()) {
                running = false;
                break;
            }
            if (handler.is<cen::keyboard_event>()) {
                const auto& keyboardEvent = handler.get<cen::keyboard_event>();
                if (!keyboardEvent.pressed()) {
                    continue;
                }
                namespace sc = cen::scancodes;
                using enum Direction;
                if (keyboardEvent.is_active(sc::right)
                    || keyboardEvent.is_active(sc::d)) {
                    mover.emplace_back(Right);
                } else if (keyboardEvent.is_active(sc::up)
                    || keyboardEvent.is_active(sc::w)) {
                    mover.emplace_back(Up);
                } else if (keyboardEvent.is_active(sc::left)
                    || keyboardEvent.is_active(sc::a)) {
                    mover.emplace_back(Left);
                } else if (keyboardEvent.is_active(sc::down)
                    || keyboardEvent.is_active(sc::s)) {
                    mover.emplace_back(Down);
                }
            }
            if (mover.size() > 2) {
                mover.erase(mover.begin() + 1);
            }
        }

        renderer_.clear();

        if (process(game_timer, mover, start_ticks)) {
            draw();
            renderer_.present();
        }

        auto const iteration_end = cen::now();
        auto const elapsed_ms = (iteration_end - iteration_start)
            / (cen::frequency() * 1000U);
        cen::thread::sleep(std::chrono::milliseconds {FRAME_DURATION_MS - elapsed_ms});
    }
}