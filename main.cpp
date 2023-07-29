#include "Game.hpp"
#include "Globals.hpp"

#include <centurion.hpp>

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
    cen::renderer renderer {window.make_renderer(cen::renderer::vsync)};

    Game game{renderer};

    game.run();

    return EXIT_SUCCESS;
}
