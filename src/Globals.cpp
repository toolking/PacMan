#include "Globals.hpp"

SDL_Window* Window = NULL;
SDL_Renderer* Renderer = NULL;
TTF_Font* Font = NULL;
TTF_Font* LittleFont = NULL;

void initialize_SDL()
{
    SDL_Init(SDL_INIT_VIDEO);
    Window = SDL_CreateWindow("PacMan", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_PRESENTVSYNC);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    Font = TTF_OpenFont("Fonts/emulogic.ttf", BOCK_SIZE_24);
    LittleFont = TTF_OpenFont("Fonts/VpPixel.ttf", 20);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
}

void close_SDL()
{
    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(Window);
	TTF_CloseFont(Font);
	TTF_CloseFont(LittleFont);
    Window = NULL;
    Renderer = NULL;
    IMG_Quit();
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();
}

void init_frames(
    const unsigned char total_frames,
    SDL_Rect sprite_clips[],
    unsigned char current_block_size /* = BlockSize32 */
)
{
    unsigned short counter = 0;
    for (unsigned char i = 0; i < total_frames; i++) {
        sprite_clips[i].x = counter;
        sprite_clips[i].y = 0;
        sprite_clips[i].w = current_block_size;
        sprite_clips[i].h = current_block_size;
        counter += current_block_size;
    }
}
