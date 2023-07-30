#pragma once

#include "Blinky.hpp"
#include "Board.hpp"
#include "Clyde.hpp"
#include "Fruit.hpp"
#include "Globals.hpp"
#include "Inky.hpp"
#include "Pac.hpp"
#include "Pinky.hpp"
#include "Sound.hpp"
#include "Texture.hpp"
#include "Timer.hpp"

#include <centurion.hpp>
#include <vector>
#include <list>

class Game
{
public:
    Game(cen::renderer const& renderer)
      : renderer_ {renderer}
      , board_ {renderer_}
      , pac_ {renderer_}
      , blinky_ {renderer_}
      , inky_ {renderer_}
      , pinky_ {renderer_}
      , clyde_ {renderer_}
      , fruit_ {renderer_}
      , ready_ {renderer_, "ready!", cen::colors::yellow}
      , game_over_texture_ {renderer_, "game  over", cen::colors::red}
    {}

    void run();

private:
    void clear_mover(std::vector<Direction>& mover);
    void clock();
    void deadly_ghost_pac_coll(Ghost& ghost);
    void deadly_pac_ghost_coll();
    void death_sound();
    void draw_little_score();
    void draw();
    void entity_collisions();
    void food();
    auto get_level() -> unsigned short;
    void increase_level();
    auto is_level_completed() -> bool;
    void mod_death_sound_statement(bool new_death_sound_statement);
    void mod_start_statement(bool new_start_statement);
    void mod_to_waka(bool new_waka);
    auto process(Timer& game_timer, std::vector<Direction>& mover, cen::u64ms& start_ticks) -> bool;
    void reset_ghosts_facing();
    void reset_ghosts_life_statement();
    void start();
    void update_difficulty();
    void update_positions(std::vector<Direction>& mover, Ghost::Status timed_status);
    void update(std::vector<Direction>& mover);

    Sound sound;

    cen::renderer_handle renderer_;
    Board board_;
    Pac pac_;
    Blinky blinky_;
    Inky inky_;
    Pinky pinky_;
    Clyde clyde_;
    Fruit fruit_;
    Timer map_animation_timer_;
    TextureFont<> ready_;
    TextureFont<> game_over_texture_;
    board_type actual_map_ = board_.map();
    bool is_game_started_ = false;
    Timer ghost_timer_;
    cen::u64ms scatter_time_ {7000};
    cen::u64ms chasing_time_ {20000};
    cen::u64ms ghost_timer_target_ {chasing_time_};
    Ghost::Status timed_status_ = Ghost::Status::Chase;
    unsigned short scorer_ = 200;
    std::list<std::tuple<Timer,Position,unsigned short>> little_score_entries_;
    cen::u64ms little_timer_target_ {1000};
    unsigned short level_ = 1;
    bool is_to_scatter_sound_ = true;
    bool is_to_waka_sound_ = true;
    Timer waka_timer_;
    bool is_to_death_pac_sound_ = true;
    unsigned char dead_ghosts_counter_ = 0;
};
