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

#include <vector>

class Game
{
public:
    Game();
    void reset_ghosts_life_statement();
    void reset_ghosts_facing();
    void start();
    void mod_start_statement(bool new_start_statement);
    void clock();
    void update_positions(std::vector<Direction>& mover, bool timed_status);
    void food();
    void entity_collisions();
    void update(std::vector<Direction>& mover);
    unsigned short get_level();
    void increase_level();
    void update_difficulty();
    bool is_level_completed();
    void clear_mover(std::vector<Direction>& mover);
    void deadly_pac_ghost_coll();
    void deadly_ghost_pac_coll(Ghost& ghost);
    void mod_to_waka(bool new_waka);
    void death_sound();
    void mod_death_sound_statement(bool new_death_sound_statement);
    void draw_little_score();
    bool process(Timer& game_timer, std::vector<Direction>& mover, unsigned short& start_ticks);
    void draw();
    Sound sound;

private:
    Board board_;
    Pac pac_;
    Blinky blinky_;
    Inky inky_;
    Pinky pinky_;
    Clyde clyde_;
    Fruit fruit_;
    Timer map_animation_timer_;
    Texture ready_;
    Texture game_over_texture_;
    Board::board_type actual_map_;
    bool is_game_started_;
    Timer ghost_timer_;
    unsigned short scatter_time_;
    unsigned short chasing_time_;
    unsigned short ghost_timer_target_;
    bool timed_status_;
    unsigned short scorer_;
    std::vector<Timer> little_score_timers_;
    std::vector<Position> little_score_positions_;
    std::vector<unsigned short> little_score_scorers_;
    unsigned short little_timer_target_;
    unsigned short level_;
    bool is_to_scatter_sound_;
    bool is_to_waka_sound_;
    Timer waka_timer_;
    bool is_to_death_pac_sound_;
    unsigned char dead_ghosts_counter_;
};
