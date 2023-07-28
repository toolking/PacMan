#pragma once

#include <centurion.hpp>

class Sound
{
public:
    Sound()
    {
        #if SDL_MIXER_VERSION_ATLEAST(2, 6, 0)
        cen::sound_effect::mix_master_volume(10);
        #else
        Mix_Volume(-1, 10);
        #endif

        #ifdef CENTURION_MOCK_FRIENDLY_MODE
        intro_.set_channel(0);
        eat_fruit_.set_channel(1);
        extra_life_.set_channel(2);
        pac_death_.set_channel(3);
        ghost_death_.set_channel(4);
        scatter_ghost_.set_channel(5);
        waka_.set_channel(6);
        #endif
    }

    void play_intro() { intro_.play(); }
    void play_eat_fruit() { eat_fruit_.play(); }
    void play_extra_life() { extra_life_.play(); }
    void play_pac_death() { pac_death_.play(); }
    void play_ghost_death() { ghost_death_.play(); }
    void play_scatter_ghost() { scatter_ghost_.play(-1); }
    void stop_scatter_ghost() { scatter_ghost_.stop(); }
    void play_waka() { waka_.play(-1); }
    void stop_waka() { waka_.stop(); }

private:
    cen::sound_effect intro_ {"Sounds/Intro.wav"};
    cen::sound_effect eat_fruit_ {"Sounds/EatFruit.wav"};
    cen::sound_effect extra_life_ {"Sounds/ExtraLife.wav"};
    cen::sound_effect pac_death_ {"Sounds/PacDeath.wav"};
    cen::sound_effect ghost_death_ {"Sounds/GhostDeath.wav"};
    cen::sound_effect scatter_ghost_ {"Sounds/ScatterGhost.wav"};
    cen::sound_effect waka_ {"Sounds/Waka.wav"};
};
