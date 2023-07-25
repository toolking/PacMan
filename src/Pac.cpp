#include "Pac.hpp"

Pac::Pac()
  : Entity(Entity::Type::PacMan)
{
    living_pac_.load_from_file("Textures/PacMan32.png");
    death_pac_.load_from_file("Textures/GameOver32.png");
    init_frames(living_pac_sprite_clips_);
    init_frames(death_pac_sprite_clips_);
    curr_living_pac_frame_ = 0;
    curr_death_pac_frame_ = 0;
    energy_status_ = false;
    dead_animation_statement_ = false;
}

void Pac::update_pos(std::vector<Direction>& mover, Board::board_type const& actual_map)
{
    for (unsigned char i = 0; i < speed(); i++) {
        short TempX = position.x;
        short TempY = position.y;
        get_possible_position(TempX, TempY, mover.at(0));
        if (!wall_collision(TempX, TempY, actual_map)) {
            update_current_living_pac_frame();
            move(mover.at(0));
            set_facing(mover.at(0));
            direction(mover.at(0));
        } else
            wall_collision_frame();
        if (mover.size() != 1 && mover.at(0) != mover.at(1)) {
            TempX = position.x;
            TempY = position.y;
            get_possible_position(TempX, TempY, mover.at(1));
            if (!wall_collision(TempX, TempY, actual_map)) {
                update_current_living_pac_frame();
                move(mover.at(1));
                set_facing(mover.at(1));
                direction(mover.at(1));
                mover.erase(mover.begin());
            }
        }
        check_wrap();
    }
}

auto Pac::food_collision(Board::board_type& actual_map) -> unsigned char
{
    float cell_x = position.x / static_cast<float>(BOCK_SIZE_24);
    float cell_y = position.y / static_cast<float>(BOCK_SIZE_24);
    for (unsigned char side_dir = 0; side_dir < 4; side_dir++) {
        Position board_pos = char_board_pos(side_dir, cell_x, cell_y);
        auto& act_pos = actual_map[BOARD_WIDTH * board_pos.y + board_pos.x];
        if (BlockType::Pellet == act_pos) {
            act_pos = BlockType::Nothing;
            return 0;
        } else if (BlockType::Energizer == act_pos) {
            act_pos = BlockType::Nothing;
            return 1;
        }
    }
    return 2;
}

auto Pac::is_energized() const -> bool
{
    return energy_status_;
}

void Pac::change_energy_status(bool energy_status)
{
    energy_status_ = energy_status;
}

void Pac::set_facing(Direction mover)
{
    switch (mover) {
    case Direction::Right:
        facing(0);
        break;
    case Direction::Up:
        facing(3);
        break;
    case Direction::Left:
        facing(2);
        break;
    case Direction::Down:
        facing(1);
        break;
    default:
        break;
    }
}

auto Pac::is_dead_animation_ended() -> bool
{
    return dead_animation_statement_;
}

void Pac::mod_dead_animation_statement(bool dead_animation_statement)
{
    dead_animation_statement_ = dead_animation_statement;
}

void Pac::update_current_living_pac_frame()
{
    curr_living_pac_frame_++;
    if (curr_living_pac_frame_ / (LIVING_PAC_FRAMES * 4) >= LIVING_PAC_FRAMES) {
        curr_living_pac_frame_ = 0;
    }
}

void Pac::reset_current_living_frame()
{
    curr_living_pac_frame_ = 0;
}

void Pac::wall_collision_frame()
{
    curr_living_pac_frame_ = 32;
}

void Pac::draw()
{
    if (is_alive()) {
        auto clip = &living_pac_sprite_clips_[curr_living_pac_frame_ / (LIVING_PAC_FRAMES * 4)];
        living_pac_.render(position.x - 4, position.y - 4, facing(), clip);
    } else {
        auto clip = &death_pac_sprite_clips_[curr_death_pac_frame_ / DEATH_PAC_FRAMES];
        death_pac_.render(position.x - 4, position.y - 4, facing(), clip);
        curr_death_pac_frame_++;
        if (curr_death_pac_frame_ / DEATH_PAC_FRAMES >= DEATH_PAC_FRAMES) {
            dead_animation_statement_ = true;
            curr_death_pac_frame_ = 0;
        }
    }
}
