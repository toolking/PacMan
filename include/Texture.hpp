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
    bool load_from_rendered_text(std::string texture_text, SDL_Color text_color, bool is_little = 0);
    void free();

    void setColor(uint8_t red, uint8_t green, uint8_t blue)
    {
        SDL_SetTextureColorMod(texture_, red, green, blue);
    }

    void setBlendMode(SDL_BlendMode blending)
    {
        SDL_SetTextureBlendMode(texture_, blending);
    }

    void setAlpha(uint8_t alpha)
    {
        SDL_SetTextureAlphaMod(texture_, alpha);
    }

    void render(short x = 0, short y = 0, unsigned char facing = 0, SDL_Rect* clip = NULL);

    int get_width()
    {
        return width_;
    }

    int get_height()
    {
        return height_;
    }

private:
    SDL_Texture* texture_ {nullptr};
    unsigned short width_ {0u};
    unsigned short height_ {0u};
};
