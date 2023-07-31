#pragma once

#include "Globals.hpp"

#include <centurion.hpp>

#include <string>

class TextureImg
{
public:
    TextureImg(cen::renderer_handle& renderer, std::string path);

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
    void render(cen::ipoint const& pos, unsigned char facing, cen::irect const& clip)
    {
        render(pos.x(), pos.y(), facing, clip);
    }
    void render(cen::ipoint const& pos)
    {
        render(pos.x(), pos.y());
    }

private:
    cen::renderer_handle& renderer_;
    cen::texture texture_;
};

template <std::size_t Size>
class TextureMultiImg : public TextureImg
{
public:
    TextureMultiImg(cen::renderer_handle& renderer, std::string path)
      : TextureImg(renderer, path)
    {}

    void render(cen::ipoint const& pos, unsigned char facing, std::size_t frame)
    {
        render(pos.x(), pos.y(), facing, frame);
    }

    void render(short x, short y, unsigned char facing, std::size_t frame)
    {
        if (frame >= Size)
        {
            throw std::out_of_range("Frame out of range");
        }
        cen::irect const clip {cen::irect::value_type(frame*BLOCK_SIZE_32), 0, BLOCK_SIZE_32, BLOCK_SIZE_32};
        TextureImg::render(x, y, facing, clip);
    }
};

template <bool IsLittle = false>
class TextureFont
{
public:
    TextureFont(cen::renderer_handle& renderer, std::string texture_text, cen::color text_color = cen::colors::white)
      : renderer_ {renderer}
      , font_ {IsLittle ? cen::font {"Fonts/VpPixel.ttf", 20} : cen::font {"Fonts/emulogic.ttf", BLOCK_SIZE_24}}
      , texture_ {renderer_.make_texture(font_.render_solid(texture_text.c_str(), text_color))}
    {}
    TextureFont(TextureFont const&) = delete;
    TextureFont(TextureFont&&) = delete;
    TextureFont& operator=(TextureFont const&) = delete;
    TextureFont& operator=(TextureFont&&) = delete;

    void set_color(cen::color const& color)
    {
        texture_.set_color_mod(color);
    }

    void set_new_text(std::string texture_text, cen::color text_color = cen::colors::white)
    {
        texture_ = renderer_.make_texture(font_.render_solid(texture_text.c_str(), text_color));
    }

    void render(short x, short y)
    {
        renderer_.render(texture_, cen::ipoint {x, y});
    }

private:
    cen::renderer_handle& renderer_;
    cen::font font_;
    cen::texture texture_;
};
