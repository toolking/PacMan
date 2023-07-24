#pragma once

#include <SDL2/SDL.h>
#include <cstdint>

class Timer
{
    using ticks_t = uint32_t;

public:
    void start()
    {
        started_ = true;
        paused_ = false;
        start_ticks_ = SDL_GetTicks();
        paused_ticks_ = 0;
    }

    void reset()
    {
        *this = Timer{};
    }

    void restart()
    {
        reset();
        start();
    }

    void pause()
    {
        if (!started_ || paused_)
            return;
        paused_ = true;
        paused_ticks_ = SDL_GetTicks() - start_ticks_;
        start_ticks_ = 0;
    }

    void unpause()
    {
        if (!started_ || !paused_)
            return;
        paused_ = false;
        start_ticks_ = SDL_GetTicks() - paused_ticks_;
        paused_ticks_ = 0;
    }

    ticks_t get_ticks() const
    {
        if (!started_) {
            return ticks_t {};
        }
        if (paused_) {
            return paused_ticks_;
        }
        return SDL_GetTicks() - start_ticks_;
    }

    bool is_started() const
    {
        return started_;
    }

    bool is_paused() const
    {
        return paused_ && started_;
    }

private:
    ticks_t start_ticks_ {}; // Ticks since start
    ticks_t paused_ticks_ {}; // Ticks since stop
    bool paused_ {};
    bool started_ {};
};
