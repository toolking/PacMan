#pragma once

#include "Globals.hpp"
#include "Position.hpp"
#include "Texture.hpp"
#include "Timer.hpp"

#include <array>
#include <ranges>
#include <string_view>

constexpr std::string_view CHAR_BOARD = "                            "
                                        "                            "
                                        "                            "
                                        "############################"
                                        "#............##............#"
                                        "#.####.#####.##.#####.####.#"
                                        "#o####.#####.##.#####.####o#"
                                        "#.####.#####.##.#####.####.#"
                                        "#..........................#"
                                        "#.####.##.########.##.####.#"
                                        "#.####.##.########.##.####.#"
                                        "#......##....##....##......#"
                                        "######.##### ## #####.######"
                                        "     #.##### ## #####.#     "
                                        "     #.##    1     ##.#     "
                                        "     #.## ###==### ##.#     "
                                        "######.## #      # ##.######"
                                        "      .   #2 3 4 #   .      "
                                        "######.## #      # ##.######"
                                        "     #.## ######## ##.#     "
                                        "     #.##          ##.#     "
                                        "     #.## ######## ##.#     "
                                        "######.## ######## ##.######"
                                        "#............##............#"
                                        "#.####.#####.##.#####.####.#"
                                        "#.####.#####.##.#####.####.#"
                                        "#o..##.......0 .......##..o#"
                                        "###.##.##.########.##.##.###"
                                        "###.##.##.########.##.##.###"
                                        "#......##....##....##......#"
                                        "#.##########.##.##########.#"
                                        "#.##########.##.##########.#"
                                        "#..........................#"
                                        "############################"
                                        "                            "
                                        "                            ";

class Entity;

enum class BlockType
{
    Nothing = 0,
    Wall,
    Door,
    Pellet,
    Energizer
};

using board_type = std::array<BlockType, BOARD_HEIGHT * BOARD_WIDTH>;

constexpr auto char_board_2_bin_board(std::string_view const& char_board) -> board_type
{
    board_type bin_board;

    std::transform(char_board.cbegin(), char_board.cend(), bin_board.begin(), [](char c) {
        switch (c) {
            using enum BlockType;
        case '#': return Wall;
        case '=': return Door;
        case '.': return Pellet;
        case 'o': return Energizer;
        default: return Nothing;
        }
    });
    return bin_board;
}

class Board
{
public:
    Board(cen::renderer_handle const& renderer)
      : renderer_ {renderer}
      , map_texture_ {renderer_, "Textures/Map24.png"}
      , pellet_texture_ {renderer_, "Textures/Pellet24.png"}
      , energizer_texture_ {renderer_, "Textures/Energizer24.png"}
      , door_texture_ {renderer_, "Textures/Door.png"}
      , lives_texture_ {renderer_, "Textures/Lives32.png"}
      , score_label_texture_ {renderer_, "Score"}
      , score_texture_ {renderer_, "0"}
      , high_score_label_texture_ {renderer_, "High Score"}
      , high_score_texture_ {renderer_, "0"}
    {
        map_texture_.set_color(cen::colors::blue);
    }
    auto map() const -> board_type;
    auto entity_start_position(Entity const&) const -> Position;
    void set_score();
    void set_high_score();
    bool is_extra_life();
    void increase_lives();
    void decrease_lives();
    void score_increase(unsigned short scorer);
    unsigned char get_lives();
    void draw(board_type const& actual_map, Timer map_animation_timer);

private:
    cen::renderer_handle renderer_;
    TextureImg map_texture_;
    TextureImg pellet_texture_;
    TextureImg energizer_texture_;
    TextureImg door_texture_;
    TextureImg lives_texture_;
    TextureFont<> score_label_texture_;
    TextureFont<> score_texture_;
    TextureFont<> high_score_label_texture_;
    TextureFont<> high_score_texture_;
    board_type numeric_board_ {char_board_2_bin_board(CHAR_BOARD)};
    unsigned int score_ {};
    bool is_extra_ {};
    unsigned int lives_ {4};
};
