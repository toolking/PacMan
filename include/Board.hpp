#pragma once

#include "Globals.hpp"
#include "Texture.hpp"
#include "Timer.hpp"
#include "Position.hpp"

#include <array>
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
    Nothing=0,
    Wall,
    Door,
    Pellet,
    Energizer
};

    using block_type = BlockType;
    using board_type = std::array<block_type, BOARD_HEIGHT * BOARD_WIDTH>;

constexpr auto char_board_2_bin_board(std::string_view char_board) -> board_type
{
    board_type bin_board;
    std::transform(char_board.begin(), char_board.end(), bin_board.begin(), [](char c) {
        switch (c) {
        case '#':
            return BlockType::Wall;
        case '=':
            return BlockType::Door;
        case '.':
            return BlockType::Pellet;
        case 'o':
            return BlockType::Energizer;
        default:
            return BlockType::Nothing;
        }
    });
    return bin_board;
}

class Board
{
public:

    Board(cen::renderer const& renderer)
      : renderer_(renderer)
      , map_texture_{renderer_}
      , pellet_texture_{renderer_}
      , energizer_texture_{renderer_}
      , door_texture_{renderer_}
      , lives_texture_{renderer_}
      , score_label_texture_{renderer_}
      , score_texture_{renderer_}
      , high_score_label_texture_{renderer_}
      , high_score_texture_{renderer_}
    {
    map_texture_.load_from_file("Textures/Map24.png");
    pellet_texture_.load_from_file("Textures/Pellet24.png");
    energizer_texture_.load_from_file("Textures/Energizer24.png");
    door_texture_.load_from_file("Textures/Door.png");
    lives_texture_.load_from_file("Textures/Lives32.png");
    score_label_texture_.load_from_rendered_text("Score", WHITE);
    high_score_label_texture_.load_from_rendered_text("High Score", WHITE);
    map_texture_.set_color(BLUE);
    numeric_board_ = char_board_2_bin_board(CHAR_BOARD);

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
    cen::renderer const& renderer_;
    Texture map_texture_;
    Texture pellet_texture_;
    Texture energizer_texture_;
    Texture door_texture_;
    Texture lives_texture_;
    Texture score_label_texture_;
    Texture score_texture_;
    Texture high_score_label_texture_;
    Texture high_score_texture_;
    board_type numeric_board_;
    unsigned int score_ {};
    bool is_extra_ {};
    unsigned int lives_ {4};
};
