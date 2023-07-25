#pragma once

#include <SDL2/SDL.h>
#include <cstdint>

class Timer
{
    using ticks_type = uint32_t;

public:
    void start()
    {
        started_ = true;
        start_ticks_ = SDL_GetTicks();
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

    ticks_type get_ticks() const
    {
        if (!started_) {
            return ticks_type {};
        }
        return SDL_GetTicks() - start_ticks_;
    }

    bool is_started() const
    {
        return started_;
    }

private:
    ticks_type start_ticks_ {}; // Ticks since start
    bool started_ {};
};
