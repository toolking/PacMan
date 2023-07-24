#pragma once

#include <SDL2/SDL_mixer.h>
#include <memory>

namespace {
inline void free_sound(Mix_Chunk* sound)
{
    Mix_FreeChunk(sound);
    sound = NULL;
}
}

class Sound
{
    using self_cleanup_sound = std::unique_ptr<Mix_Chunk,decltype(&free_sound)>;
    
public:

    Sound()
    {
        Mix_Volume(-1, 10);
    }

    void play_intro() { Mix_PlayChannel(0, intro_.get(), 0); }
    void play_eat_fruit() { Mix_PlayChannel(1, eat_fruit_.get(), 0); }
    void play_extra_life() { Mix_PlayChannel(2, extra_life_.get(), 0); }
    void play_pac_death() { Mix_PlayChannel(3, pac_death_.get(), 0); }
    void play_ghost_death() { Mix_PlayChannel(4, ghost_death_.get(), 0); }
    void play_scatter_ghost() { Mix_PlayChannel(5, scatter_ghost_.get(), -1); }
    void stop_scatter_ghost() { Mix_HaltChannel(5); }
    void play_waka() { Mix_PlayChannel(6, waka_.get(), -1); }
    void stop_waka() { Mix_HaltChannel(6); }

private:
    self_cleanup_sound intro_ {Mix_LoadWAV("Sounds/Intro.wav"),free_sound};
    self_cleanup_sound eat_fruit_ {Mix_LoadWAV("Sounds/EatFruit.wav"),free_sound};
    self_cleanup_sound extra_life_ {Mix_LoadWAV("Sounds/ExtraLife.wav"),free_sound};
    self_cleanup_sound pac_death_ {Mix_LoadWAV("Sounds/PacDeath.wav"),free_sound};
    self_cleanup_sound ghost_death_ {Mix_LoadWAV("Sounds/GhostDeath.wav"),free_sound};
    self_cleanup_sound scatter_ghost_ {Mix_LoadWAV("Sounds/ScatterGhost.wav"),free_sound};
    self_cleanup_sound waka_ {Mix_LoadWAV("Sounds/Waka.wav"),free_sound};
};
