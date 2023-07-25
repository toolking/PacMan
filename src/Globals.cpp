#include "Globals.hpp"

SDL_Window* Window = nullptr;
SDL_Renderer* Renderer = nullptr;
TTF_Font* Font = nullptr;
TTF_Font* LittleFont = nullptr;

void initialize_SDL()
{
    SDL_Init(SDL_INIT_VIDEO);
    Window = SDL_CreateWindow("PacMan", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_PRESENTVSYNC);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    Font = TTF_OpenFont("Fonts/emulogic.ttf", BLOCK_SIZE_24);
    LittleFont = TTF_OpenFont("Fonts/VpPixel.ttf", 20);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
}

void close_SDL()
{
    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(Window);
    TTF_CloseFont(Font);
    TTF_CloseFont(LittleFont);
    Window = nullptr;
    Renderer = nullptr;
    IMG_Quit();
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();
}
