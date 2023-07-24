#include "Game.hpp"
#include "Globals.hpp"

#include <vector>

// Use arrow keys or WASD to move

auto main(int, char*[]) -> int
{
    initialize_SDL();

    Game game;
    Timer GameTimer;
    SDL_Event event;
    bool quit = false;
    unsigned short StartTicks = 4500;
    std::vector<Direction> mover;
    mover.push_back(Direction::Right);
    GameTimer.start();
    game.sound.play_intro();

    while (!quit) {
        uint64_t IterationStart = SDL_GetPerformanceCounter();

        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT)
                quit = true;
            if (event.key.state == SDL_PRESSED) {
                if ((event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d))
                    mover.push_back(Direction::Right);
                else if ((event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w))
                    mover.push_back(Direction::Up);
                else if ((event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a))
                    mover.push_back(Direction::Left);
                else if ((event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s))
                    mover.push_back(Direction::Down);
                if (mover.size() > 2)
                    mover.erase(mover.begin() + 1);
            }
        }

        SDL_RenderClear(Renderer);

        if (game.process(GameTimer, mover, StartTicks)) {
            game.draw();
            SDL_RenderPresent(Renderer);
        }

        uint64_t IterationEnd = SDL_GetPerformanceCounter();
        float elapsedMS = (IterationEnd - IterationStart) / ((float)SDL_GetPerformanceFrequency() * 1000.0f);
        SDL_Delay(floor(11.111f - elapsedMS));
    }

    close_SDL();

    return 0;
}
