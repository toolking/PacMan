#include "Game.hpp"
#include "Globals.hpp"
#include "Timer.hpp"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <cmath>
#include <cstdint>
#include <vector>

// Use arrow keys or WASD to move

auto main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) -> int
{
    initialize_SDL();

    Game game;
    Timer game_timer;
    SDL_Event event;
    bool quit = false;
    unsigned short start_ticks = START_WAIT_TICKS;
    std::vector<Direction> mover;
    mover.push_back(Direction::Right);
    game_timer.start();
    game.sound.play_intro();

    while (!quit) {
        uint64_t const iteration_start {SDL_GetPerformanceCounter()};

        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            if (event.key.state != SDL_PRESSED) {
                continue;
            }
            if ((event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)) {
                mover.emplace_back(Direction::Right);
            } else if ((event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)) {
                mover.emplace_back(Direction::Up);
            } else if ((event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)) {
                mover.emplace_back(Direction::Left);
            } else if ((event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)) {
                mover.emplace_back(Direction::Down);
            }
            if (mover.size() > 2) {
                mover.erase(mover.begin() + 1);
            }
        }

        SDL_RenderClear(Renderer);

        if (game.process(game_timer, mover, start_ticks)) {
            game.draw();
            SDL_RenderPresent(Renderer);
        }

        uint64_t const iteration_end = SDL_GetPerformanceCounter();
        float const elapsed_ms = static_cast<float>(iteration_end - iteration_start) / (static_cast<float>(SDL_GetPerformanceFrequency() * 1000U));
        SDL_Delay(floor(11.111F - elapsed_ms));
    }

    close_SDL();

    return 0;
}
