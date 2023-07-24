#include "Texture.hpp"
#include "Globals.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

Texture::~Texture()
{
    free();
}

bool Texture::load_from_file(std::string path)
{
    // Get rid of preexisting texture
    free();

    // The final texture
    SDL_Texture* new_texture = NULL;

    // Load image at specified path
    SDL_Surface* loaded_surface = IMG_Load(path.c_str());
    if (loaded_surface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
        goto out_load_no_load;
    }
    // Color key image
    SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGB(loaded_surface->format, 0, 0xFF, 0xFF));

    // Create texture from surface pixels
    new_texture = SDL_CreateTextureFromSurface(Renderer, loaded_surface);
    if (new_texture == NULL) {
        printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        goto out_load_no_texture;
    }
    // Get image dimensions
    width_ = loaded_surface->w;
    height_ = loaded_surface->h;

out_load_no_texture:
    // Get rid of old loaded surface
    SDL_FreeSurface(loaded_surface);
out_load_no_load:
    // Return success
    texture_ = new_texture;
    return texture_ != NULL;
}

bool Texture::load_from_rendered_text(std::string texture_text, SDL_Color text_color, bool is_little)
{
    // Get rid of preexisting texture
    free();

    // Render text surface
    SDL_Surface* text_surface = TTF_RenderText_Solid(is_little ? LittleFont : Font, texture_text.c_str(), text_color);
    if (text_surface == NULL) {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        goto out_text_no_render;
    }
    // Create texture from surface pixels
    texture_ = SDL_CreateTextureFromSurface(Renderer, text_surface);
    if (texture_ == NULL) {
        printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        goto out_text_no_texture;
    }

    // Get image dimensions
    width_ = text_surface->w;
    height_ = text_surface->h;

out_text_no_texture:
    // Get rid of old surface
    SDL_FreeSurface(text_surface);
out_text_no_render:
    // Return success
    return texture_ != NULL;
}

void Texture::free()
{
    if (texture_ == NULL) return;
    SDL_DestroyTexture(texture_);
    texture_ = NULL;
    width_ = 0;
    height_ = 0;
}

void Texture::render(short x, short y, unsigned char facing, SDL_Rect* clip)
{
    // Set rendering space and render to screen
    SDL_Rect render_quad = {x, y, width_, height_};

    // Set clip rendering dimensions
    if (clip != NULL) {
        render_quad.w = clip->w;
        render_quad.h = clip->h;
    }
    // Render to screen
    SDL_RenderCopyEx(Renderer, texture_, clip, &render_quad, facing * 90.0f, NULL, SDL_FLIP_NONE);
}
