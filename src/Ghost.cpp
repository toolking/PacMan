#include "Ghost.hpp"

inline auto direction2facing(Direction d) -> unsigned int
{
    using enum Direction;
    switch (d) {
    case Right: return 0;
    case Up: return 1;
    case Left: return 2;
    case Down: return 3;
    default: return 4;
    }
}

Ghost::Ghost(cen::renderer_handle& renderer, cen::color color, Entity::Type identity)
  : Entity {identity}
  , renderer_ {renderer}
  , body_ {renderer_, "Textures/GhostBody32.png"}
  , eyes_ {renderer_, "Textures/GhostEyes32.png"}
  , color_ {color}
{}

auto Ghost::is_target_to_calculate(Pac const& pac) -> bool
{
    if (!is_alive) {
        can_use_door_ = true;
        Target = Home;
        if (position == Home) {
            is_alive = true;
        }
        return false;
    }

    if (is_home() && pac.is_energized()) {
        if (position == Home) {
            Target.set_y(Home.y() - BLOCK_SIZE_24);
        } else if (position.x() == Home.x() && position.y() == Home.y() - BLOCK_SIZE_24) {
            Target.set_y(Home.y());
        }
        return false;
    }

    if (is_home() && is_alive) {
        can_use_door_ = true;
        Target = DoorTarget;
        return false;
    }

    can_use_door_ = false;
    if (status_) {
        Target = ScatterTarget;
        return false;
    }
    return true;
}

void Ghost::poss_dirs_bubble_sort(std::vector<float>& distances, std::vector<Direction>& possible_directions)
{
    for (auto i = 0U; i < distances.size(); i++) {
        for (auto j = 0U; j < distances.size(); j++) {
            if (distances[i] < distances[j]) {
                std::swap(distances[j], distances[i]);
                std::swap(possible_directions[j], possible_directions[i]);
            }
        }
    }
}

void Ghost::calculate_direction(board_type const& actual_map)
{
    std::vector<float> distances;
    std::vector<Direction> possible_directions;
    using enum Direction;
    for (Direction i : {Right, Up, Left, Down}) {
        auto const pos = get_possible_position(position, i);
        if (!wall_collision(pos, actual_map, can_use_door_)) {
            float dist_x = abs(pos.x() - Target.x());
            dist_x = (dist_x > WINDOW_WIDTH / 2)?WINDOW_WIDTH - dist_x:dist_x;
            auto dist = static_cast<float>(sqrt(pow(dist_x, 2) + pow(pos.y() - Target.y(), 2)));
            distances.push_back(dist);
            possible_directions.push_back(i);
        }
    }

    if (possible_directions.size() == 1) {
        direction = possible_directions[0];
        return;
    }

    poss_dirs_bubble_sort(distances, possible_directions);

    for (auto const& dir : possible_directions) {
        if (dir != -direction) {
            direction = dir;
            return;
        }
    }
}

auto Ghost::is_home() -> bool
{
    constexpr cen::irect home {11 * BLOCK_SIZE_24, 15 * BLOCK_SIZE_24, 6 * BLOCK_SIZE_24, 3 * BLOCK_SIZE_24};
    return home.contains(position);
}

void Ghost::mod_status(bool status)
{
    status_ = status;
}

void Ghost::update_status(Pac const& pac, bool timed_status)
{
    status_ = pac.is_energized()?true:timed_status;
}

void Ghost::update_facing(Pac const& pac)
{
    if (is_home()) {
        return (direction == Direction::Down) ? facing(3) : facing(1);
    }

    if (pac.is_energized()) {
        return (!is_alive) ? facing(direction2facing(direction)) : facing(4);
    }

    facing(direction2facing(direction));
}

void Ghost::update_speed(Pac const& pac)
{
    speed = (!is_alive) ? 6 : pac.is_energized() ? 1
                                                 : 2;
}

void Ghost::draw(Pac const& pac, Timer ghost_timer, cen::u64ms timer_target)
{
    using namespace std::chrono_literals;
    body_.set_color(color_);
    eyes_.set_color(cen::colors::white);

    if (pac.is_energized() && is_alive && !is_home()) {
        body_.set_color(cen::colors::blue);
        if (ghost_timer.get_ticks() > timer_target - 2000ms) {
            if ((ghost_timer.get_ticks() / 250) % 2 == 1ms) {
                body_.set_color(cen::colors::white);
                eyes_.set_color(cen::colors::red);
            }
        }
    }

    if (is_alive) {
        auto const clip = ghost_body_sprite_clips_[current_body_frame_ / GHOST_BODY_FRAMES];
        body_.render(position.x() - 4, position.y() - 4, 0, clip);
    }
    auto const clip = ghost_eye_sprite_clips_[facing()];
    eyes_.render(position.x() - 4, position.y() - 4, 0, clip);
    current_body_frame_++;
    current_body_frame_ %= GHOST_BODY_FRAMES;
}
