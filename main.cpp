#include "Game.hpp"
#include "Globals.hpp"
#include "Timer.hpp"

#include <centurion.hpp>

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <cmath>
#include <cstdint>
#include <vector>

SDL_Renderer* Renderer = nullptr;
TTF_Font* Font = nullptr;
TTF_Font* LittleFont = nullptr;

// Use arrow keys or WASD to move

auto main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) -> int
{
    const cen::sdl sdl {{
        .flags = SDL_INIT_VIDEO
    }};
    const cen::img img {{
        .flags = IMG_INIT_PNG
    }};
    const cen::ttf ttf;
    const cen::mix mix {{
        .frequency = 44100,
        .format = MIX_DEFAULT_FORMAT,
        .channels = 2,
        .chunk_size = 1024
    }};

    cen::window window {"PacMan", {WINDOW_WIDTH, WINDOW_HEIGHT}, cen::window::shown};
    cen::renderer renderer = window.make_renderer(cen::renderer::vsync);
    Renderer = renderer.get();
    cen::font font {"Fonts/emulogic.ttf", BLOCK_SIZE_24};
    cen::font little_font {"Fonts/VpPixel.ttf", 20};
    Font = font.get();
    LittleFont = little_font.get();

    Game game{renderer};
    Timer game_timer;
    unsigned short start_ticks = START_WAIT_TICKS;
    std::vector<Direction> mover;
    mover.push_back(Direction::Right);
    game_timer.start();
    game.sound.play_intro();

    cen::event_handler handler;
    bool running = true;
    while (running) {
        auto const iteration_start {cen::now()};
        while (handler.poll()) {
            if (handler.is<cen::quit_event>()) {
                running = false;
                break;
            }
            if (handler.is<cen::keyboard_event>()) {
                const auto& keyboardEvent = handler.get<cen::keyboard_event>();
                if (!keyboardEvent.pressed()) {
                    continue;
                }
                namespace sc = cen::scancodes;
                if (   keyboardEvent.is_active(sc::right)
                    || keyboardEvent.is_active(sc::d)) {
                    mover.emplace_back(Direction::Right);
                }
                else if (   keyboardEvent.is_active(sc::up)
                         || keyboardEvent.is_active(sc::w)) {
                    mover.emplace_back(Direction::Up);
                }
                else if (   keyboardEvent.is_active(sc::left)
                         || keyboardEvent.is_active(sc::a)) {
                    mover.emplace_back(Direction::Left);
                }
                else if (   keyboardEvent.is_active(sc::down)
                         || keyboardEvent.is_active(sc::s)) {
                    mover.emplace_back(Direction::Down);
                }
            }
            if (mover.size() > 2) {
                mover.erase(mover.begin() + 1);
            }
        }

        renderer.clear();

        if (game.process(game_timer, mover, start_ticks)) {
            game.draw();
            renderer.present();
        }

        auto const iteration_end = cen::now();
        auto const elapsed_ms = (iteration_end - iteration_start)
            / (cen::frequency() * 1000U);
        cen::thread::sleep(std::chrono::milliseconds{FRAME_DURATION_MS - elapsed_ms});
    }

    return EXIT_SUCCESS;
}
