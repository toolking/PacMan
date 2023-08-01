#include "Ghost.hpp"

namespace {

auto direction2facing(Direction d) -> unsigned int
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

void order_by_distance(std::vector<std::pair<float,Direction>>& pos)
{
    for (auto i = 0U; i < pos.size(); i++) {
        for (auto j = 0U; j < pos.size(); j++) {
            if (pos[i].first < pos[j].first) {
                std::swap(pos[j], pos[i]);
            }
        }
    }
}
} // namespace

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
    if (status_ == Status::Scatter) {
        Target = ScatterTarget;
        return false;
    }
    return true;
}

void Ghost::calculate_direction(board_type const& actual_map)
{
    std::vector<std::pair<float,Direction>> possibilities;
    using enum Direction;
    for (Direction const i : {Right, Up, Left, Down}) {
        auto const pos = get_possible_position(position, i);
        if (!wall_collision(pos, actual_map, can_use_door_)) {
            possibilities.emplace_back(wrapped_distance(pos, Target), i);
        }
    }

    if (possibilities.size() == 1) {
        direction = possibilities[0].second;
        return;
    }

    order_by_distance(possibilities);

    for (auto const& [dist, dir] : possibilities) {
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

void Ghost::update_status(Pac const& pac, Status timed_status)
{
    status_ = pac.is_energized()
                ?Status::Scatter
                :timed_status;
}

void Ghost::update_facing(Pac const& pac)
{
    if (is_home()) {
        facing = (direction == Direction::Down) ? 3 : 1;
        return;
    }

    if (pac.is_energized()) {
        facing = (!is_alive) ? direction2facing(direction) : 4;
        return;
    }

    facing = direction2facing(direction);
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
        if (ghost_timer.get_ticks() > timer_target - 2s) {
            if ((ghost_timer.get_ticks() / 250) % 2 == 1ms) {
                body_.set_color(cen::colors::white);
                eyes_.set_color(cen::colors::red);
            }
        }
    }

    auto const renderpos = position - cen::ipoint{4,4};
    if (is_alive) {
        body_.render(renderpos, 0, current_body_frame_/GHOST_BODY_FRAMES);
    }
    eyes_.render(renderpos, 0, facing);

    current_body_frame_++;
    if (current_body_frame_ / GHOST_BODY_FRAMES >= GHOST_BODY_FRAMES) {
        current_body_frame_ = 0;
    }
}
