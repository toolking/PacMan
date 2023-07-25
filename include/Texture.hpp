#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_image.h>
#include <string>

class Texture
{
public:
    Texture() = default;
    ~Texture();
    bool load_from_file(std::string path);
    bool load_from_rendered_text(std::string texture_text, SDL_Color text_color, bool is_little = false);
    void set_color(uint8_t red, uint8_t green, uint8_t blue)
    {
        SDL_SetTextureColorMod(texture_, red, green, blue);
    }

    void render(short x, short y, unsigned char facing = 0, SDL_Rect* clip = nullptr);

private:
    void free();

    SDL_Texture* texture_ {nullptr};
    unsigned short width_ {0u};
    unsigned short height_ {0u};
};
