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
#include <chrono>

using namespace std::chrono_literals;

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
      , game_over_texture_ {renderer_, "game  over", cen::colors::red}
      , ready_ {renderer_, "ready!", cen::colors::yellow}
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
    auto is_level_completed() -> bool;
    //void mod_death_sound_statement(bool new_death_sound_statement);
    //void mod_start_statement(bool new_start_statement);
    //void mod_to_waka(bool new_waka);
    auto process(std::vector<Direction>& mover, cen::u64ms& start_ticks) -> bool;
    void reset_ghosts();
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

    TextureFont<> game_over_texture_;
    TextureFont<> ready_;

    board_type actual_map_ {Board::BIN_BOARD};
    bool is_game_started_ {false};
    bool is_to_death_pac_sound_ {true};
    bool is_to_scatter_sound_ {true};
    bool is_to_waka_sound_ {true};
    cen::u64ms chasing_time_ {20s};
    cen::u64ms ghost_timer_target_ {chasing_time_};
    cen::u64ms little_timer_target_ {1s};
    cen::u64ms scatter_time_ {7s};
    Ghost::Status timed_status_ {Ghost::Status::Chase};
    std::list<std::tuple<Timer,cen::ipoint,unsigned short>> little_score_entries_;
    Timer game_timer_;
    Timer ghost_timer_;
    Timer map_animation_timer_;
    Timer waka_timer_;
    unsigned char dead_ghosts_counter_ {0};
    unsigned short level_ {1};
    unsigned short scorer_ {200};
};
