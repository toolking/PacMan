#pragma once

#include <centurion.hpp>

#include <string>

class Texture
{
public:
    explicit Texture(cen::renderer const& renderer) : renderer_(renderer) {}
    //Texture(Texture const&) = delete;
    //Texture(Texture&&) = delete;
    //Texture& operator=(Texture const&) = delete;
    //Texture& operator=(Texture&&) = delete;

    ~Texture();
    bool load_from_file(std::string path);
    bool load_from_rendered_text(std::string texture_text, SDL_Color text_color, bool is_little = false);

    void set_color(SDL_Color const& color)
    {
        SDL_SetTextureColorMod(texture_, color.r, color.g, color.b);
    }

    void render(short x, short y, unsigned char facing = 0, SDL_Rect* clip = nullptr);

private:
    void free();
    cen::renderer const& renderer_;
    SDL_Texture* texture_ {nullptr};
    unsigned short width_ {0u};
    unsigned short height_ {0u};
};
