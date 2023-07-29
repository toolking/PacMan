#pragma once

#include "Globals.hpp"

#include <centurion.hpp>

#include <string>

class TextureImg
{
public:
    TextureImg(cen::renderer_handle const& renderer, std::string path);

    TextureImg(TextureImg const&) = delete;
    TextureImg(TextureImg&&) = delete;
    TextureImg& operator=(TextureImg const&) = delete;
    TextureImg& operator=(TextureImg&&) = delete;

    void set_color(cen::color const& color)
    {
        texture_.set_color_mod(color);
    }

    void render(short x, short y, unsigned char facing, cen::irect const& clip);
    void render(short x, short y);

private:
    cen::renderer_handle renderer_;
    cen::surface surface_;
    cen::texture texture_;
};

template <bool IsLittle = false>
class TextureFont
{
public:
    TextureFont(cen::renderer_handle const& renderer, std::string texture_text, cen::color text_color)
      : renderer_ {renderer}
      , font_ {IsLittle ? cen::font {"Fonts/VpPixel.ttf", 20} : cen::font {"Fonts/emulogic.ttf", BLOCK_SIZE_24}}
      , surface_ {font_.render_solid(texture_text.c_str(), text_color)}
      , texture_ {renderer_.make_texture(surface_)}
    {}
    TextureFont(TextureFont const&) = delete;
    TextureFont(TextureFont&&) = delete;
    TextureFont& operator=(TextureFont const&) = delete;
    TextureFont& operator=(TextureFont&&) = delete;

    void set_color(cen::color const& color)
    {
        texture_.set_color_mod(color);
    }

    void set_new_text(std::string texture_text, cen::color text_color)
    {
        surface_ = font_.render_solid(texture_text.c_str(), text_color);
        texture_ = renderer_.make_texture(surface_);
    }

    void render(short x, short y)
    {
        renderer_.render(texture_, cen::ipoint{x, y});
    }

private:
    cen::renderer_handle renderer_;
    cen::font font_;
    cen::surface surface_;
    cen::texture texture_;
};
