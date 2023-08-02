#pragma once

#include "Globals.hpp"
#include "Position.hpp"
#include "Texture.hpp"
#include "Timer.hpp"

#include <array>
#include <fstream>
#include <ranges>
#include <string_view>

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
        std::ifstream high_score_file("HighScore.txt");
        high_score_file >> high_score_;
        high_score_texture_.set_new_text(std::to_string(high_score_), cen::colors::white);
        high_score_file.close();
    }
    void set_score();
    bool is_extra_life();
    void decrease_lives();
    void score_increase(unsigned short scorer);
    unsigned char get_lives();
    void draw(board_type const& actual_map, Timer map_animation_timer);
    static constexpr std::string_view CHAR_BOARD {
    //   0         1         2         3
        "                            " // 0
        "                            "
        "                            "
        "############################"
        "#............##............#"
        "#.####.#####.##.#####.####.#"
        "#o####.#####.##.#####.####o#"
        "#.####.#####.##.#####.####.#"
        "#..........................#"
        "#.####.##.########.##.####.#"
        "#.####.##.########.##.####.#" // 1
        "#......##....##....##......#"
        "######.##### ## #####.######"
        "     #.##### ## #####.#     "
        "     #.##    1     ##.#     "
        "     #.## ###==### ##.#     "
        "######.## #      # ##.######"
        "      .   #2 3 4 #   .      "
        "######.## #      # ##.######"
        "     #.## ######## ##.#     "
        "     #.##          ##.#     " // 2
        "     #.## ######## ##.#     "
        "######.## ######## ##.######"
        "#............##............#"
        "#.####.#####.##.#####.####.#"
        "#.####.#####.##.#####.####.#"
        "#o..##.......0 .......##..o#"
        "###.##.##.########.##.##.###"
        "###.##.##.########.##.##.###"
        "#......##....##....##......#"
        "#.##########.##.##########.#" // 3
        "#.##########.##.##########.#"
        "#..........................#"
        "############################"
        "                            "
        "                            "};
    static constexpr board_type BIN_BOARD {char_board_2_bin_board(CHAR_BOARD)};

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
    unsigned int score_ {};
    unsigned int high_score_ {};
    bool is_extra_ {};
    unsigned int lives_ {4};
};
