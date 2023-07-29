#pragma once

#include <SDL2/SDL.h>
#include <cstdint>

class Timer
{
    using ticks_type = cen::u64ms;

public:
    void start()
    {
        started_ = true;
        start_ticks_ = cen::ticks64();
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
        return cen::ticks64() - start_ticks_;
    }

    bool is_started() const
    {
        return started_;
    }

private:
    ticks_type start_ticks_ {}; // Ticks since start
    bool started_ {};
};
