#include "Board.hpp"

#include "Entity.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string_view>

constexpr std::string_view CharBoard = "                            "
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

Board::Board()
{
    map_texture_.load_from_file("Textures/Map24.png");
    pellet_texture_.load_from_file("Textures/Pellet24.png");
    energizer_texture_.load_from_file("Textures/Energizer24.png");
    door_texture_.load_from_file("Textures/Door.png");
    lives_texture_.load_from_file("Textures/Lives32.png");
    score_word_texture_.load_from_rendered_text("Score", WHITE);
    high_score_word_texture_.load_from_rendered_text("High Score", WHITE);
    map_texture_.setColor(0, 0, 255);
    convert_sketch();
}

Board::~Board()
{
    map_texture_.free();
    pellet_texture_.free();
    energizer_texture_.free();
    door_texture_.free();
    lives_texture_.free();
    score_word_texture_.free();
    score_texture_.free();
    high_score_word_texture_.free();
    high_score_texture_.free();
}

void Board::convert_sketch()
{
    std::transform(CharBoard.begin(), CharBoard.end(), numeric_board_.begin(), [](char c) {
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
}

void Board::reset_position(Entity& entity)
{
    char y = -1;
    for (unsigned short i = 0; i < BOARD_HEIGHT * BOARD_WIDTH; i++) {
        unsigned char x = i % BOARD_WIDTH;
        if (x == 0)
            y++;
        if (   (CharBoard[i] == '0' && entity.identity() == EntityType::ePacMan)
            || (CharBoard[i] == '1' && entity.identity() == EntityType::eBlinky)
            || (CharBoard[i] == '2' && entity.identity() == EntityType::eInky)
            || (CharBoard[i] == '3' && entity.identity() == EntityType::ePinky)
            || (CharBoard[i] == '4' && entity.identity() == EntityType::eClyde)) {
            entity.position(Position(x * BOCK_SIZE_24 + BOCK_SIZE_24 / 2, y * BOCK_SIZE_24));
            return;
        }
    }
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
    } else
        ss << high;
    high_score_texture_.load_from_rendered_text(ss.str(), WHITE);
    high_scores.close();
}

void Board::draw(board_type const& actual_map, Timer map_animation_timer)
{
    score_word_texture_.render();
    score_texture_.render(0, BLOCK_SIZE_32);
    high_score_word_texture_.render(336);
    high_score_texture_.render(336, BLOCK_SIZE_32);
    map_texture_.render();
    for (unsigned char i = 1; i <= lives_; i++) {
        lives_texture_.render(i * BLOCK_SIZE_32, 26 * BLOCK_SIZE_32 - BLOCK_SIZE_32 / 4);
    }
    if (!map_animation_timer.is_started()) {
        door_texture_.render(WINDOW_WIDTH / 2 - 23, WINDOW_HEIGHT / 2 - 57);
        char y = -1;
        for (unsigned short i = 0; i < BOARD_HEIGHT * BOARD_WIDTH; i++) {
            unsigned char x = i % BOARD_WIDTH;
            if (x == 0)
                y++;
            if (actual_map[i] == BlockType::Pellet)
                pellet_texture_.render(x * BOCK_SIZE_24, y * BOCK_SIZE_24);
            if (actual_map[i] == BlockType::Energizer)
                energizer_texture_.render(x * BOCK_SIZE_24, y * BOCK_SIZE_24);
        }
    } else {
        if ((map_animation_timer.get_ticks() / 250) % 2 == 1)
            map_texture_.setColor(255, 255, 255);
        else
            map_texture_.setColor(0, 0, 255);
    }
}

void Board::copy_board(board_type& actual_map)
{
    std::copy(numeric_board_.begin(), numeric_board_.end(), actual_map.begin());
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
