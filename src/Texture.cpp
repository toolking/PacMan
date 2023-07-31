#include "Texture.hpp"
#include "Globals.hpp"

#include <fmt/format.h>

TextureImg::TextureImg(cen::renderer_handle& renderer, std::string path)
  : renderer_ {renderer}
  , surface_ {path}
  , texture_ {renderer_.make_texture(surface_)}
{}

void TextureImg::render(short x, short y, unsigned char facing, cen::irect const& clip)
{
    cen::irect const render_quad {x, y, clip.width(), clip.height()};
    renderer_.render(texture_, clip, render_quad, facing * 90.0f);
}

void TextureImg::render(short x, short y)
{
    renderer_.render(texture_, cen::ipoint{x, y});
}
