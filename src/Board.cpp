#include "Board.hpp"

#include "Entity.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string_view>
#include <unordered_map>

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

constexpr auto char_board_2_bin_board(std::string_view char_board) -> Board::board_type
{
    Board::board_type bin_board;
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

Board::Board()
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

auto Board::entity_start_position(Entity const& e) const -> Position
{
    using enum Entity::Type;
    auto const et {e.identity()};
    char const c = (et == PacMan) ? '0' 
                 : (et == Blinky) ? '1'
                 : (et == Inky)   ? '2'
                 : (et == Pinky)  ? '3'
                 : (et == Clyde)  ? '4'
                                  : ' ';
    auto const i = CHAR_BOARD.find(c);
    unsigned char const x = i % BOARD_WIDTH;
    unsigned char const y = i / BOARD_WIDTH;
    return Position(x * BLOCK_SIZE_24 + BLOCK_SIZE_24 / 2, y * BLOCK_SIZE_24);
}

void Board::set_score()
{
    std::stringstream ss;
    ss << score_;
    score_texture_.load_from_rendered_text(ss.str(), WHITE);
}

void Board::set_high_score()
{
    unsigned int high;
    std::ifstream high_scores("HighScore.txt");
    high_scores >> high;
    std::stringstream ss;
    if (score_ > high) {
        ss << score_;
        std::ofstream high_scores("HighScore.txt");
        high_scores << ss.str();
    } else {
        ss << high;
    }
    high_score_texture_.load_from_rendered_text(ss.str(), WHITE);
    high_scores.close();
}

void Board::draw(board_type const& actual_map, Timer map_animation_timer)
{
    map_texture_.render(0, 0);
    score_label_texture_.render(0, 0);
    score_texture_.render(0, BLOCK_SIZE_32);
    high_score_label_texture_.render(WINDOW_WIDTH / 2, 0);
    high_score_texture_.render(WINDOW_WIDTH / 2, BLOCK_SIZE_32);
    for (unsigned char i = 1; i <= lives_; i++) {
        lives_texture_.render(i * BLOCK_SIZE_32, 26 * BLOCK_SIZE_32 - BLOCK_SIZE_32 / 4);
    }
    if (!map_animation_timer.is_started()) {
        door_texture_.render(WINDOW_WIDTH / 2 - 23, WINDOW_HEIGHT / 2 - 57);
        //char y = -1;
        unsigned short i = 0;
        for (auto const& c: actual_map) {
            if ( c==BlockType::Pellet )
                pellet_texture_.render(i%BOARD_WIDTH * BLOCK_SIZE_24, i/BOARD_WIDTH * BLOCK_SIZE_24);
            else if (c==BlockType::Energizer)
                energizer_texture_.render(i%BOARD_WIDTH * BLOCK_SIZE_24, i/BOARD_WIDTH * BLOCK_SIZE_24);
            i++;
        }
    } else {
        if ((map_animation_timer.get_ticks() / 250) % 2 == 1)
            map_texture_.set_color(WHITE);
        else
            map_texture_.set_color(BLUE);
    }
}

auto Board::map() const -> Board::board_type
{
    return numeric_board_;
}

auto Board::is_extra_life() -> bool
{
    if (is_extra_ || score_ < 10000) {
        return false;
    }
    is_extra_ = true;
    lives_++;
    return true;
}

void Board::increase_lives()
{
    lives_++;
}

void Board::decrease_lives()
{
    lives_--;
}

auto Board::get_lives() -> unsigned char
{
    return lives_;
}

void Board::score_increase(unsigned short scorer)
{
    switch (scorer) {
    case 0:
        score_ += 10;
        break;
    case 1:
        score_ += 50;
        break;
    default:
        score_ += scorer;
        break;
    }
}
