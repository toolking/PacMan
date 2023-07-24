#include "Ghost.hpp"

inline unsigned int direction2facing(Direction d)
{
    switch (d) {
    case Direction::Right:
        return 0;
    case Direction::Up:
        return 1;
    case Direction::Left:
        return 2;
    case Direction::Down:
        return 3;
    default:
        return 4;
    }
}

Ghost::Ghost(SDL_Color color, EntityType identity)
  : Entity(identity)
{
    body_.load_from_file("Textures/GhostBody32.png");
    eyes_.load_from_file("Textures/GhostEyes32.png");
    init_frames(GHOST_BODY_FRAMES, ghost_body_sprite_clips_);
    init_frames(GHOST_EYE_FRAMES, ghost_eye_sprite_clips_);
    color_ = color;
    current_body_frame_ = 0;
    can_use_door_ = false;
    status_ = false;
    DoorTarget = Position(13 * BOCK_SIZE_24 + BOCK_SIZE_24 / 2, 15 * BOCK_SIZE_24);
}

Ghost::~Ghost()
{
    body_.free();
    eyes_.free();
}

bool Ghost::is_target_to_calculate(Pac const& pac)
{
    if (!is_alive()) {
        can_use_door_ = true;
        Target = Home;
        if (position() == Home)
            life_statement(true);
        return false;
    }

    if (is_home() && pac.is_energized()) {
        if (position() == Home)
            Target.y = Home.y - BOCK_SIZE_24;
        else if (position().x == Home.x && position().y == Home.y - BOCK_SIZE_24)
            Target.y = Home.y;
        return false;
    }

    if (is_home() && is_alive()) {
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
    for (unsigned char i = 0; i < distances.size(); i++) {
        for (unsigned char j = 0; j < distances.size(); j++) {
            if (distances.at(i) < distances.at(j)) {
                float temp1 = distances.at(i);
                distances.at(i) = distances.at(j);
                distances.at(j) = temp1;
                std::swap(possible_directions.at(j), possible_directions.at(i));
            }
        }
    }
}

void Ghost::calculate_direction(Board::board_type const& actual_map)
{
    std::vector<float> distances;
    std::vector<Direction> possible_directions;
    for (Direction i : {Direction::Right, Direction::Up, Direction::Left, Direction::Down}) {
        short pos_x = position().x;
        short pos_y = position().y;
        get_possible_position(pos_x, pos_y, i);
        if (!wall_collision(pos_x, pos_y, actual_map, can_use_door_)) {
            float dist_x = abs(pos_x - Target.x);
            if (dist_x > WINDOW_WIDTH / 2)
                dist_x = WINDOW_WIDTH - dist_x;
            float dist = static_cast<float>(sqrt(pow(dist_x, 2) + pow(pos_y - Target.y, 2)));
            distances.push_back(dist);
            possible_directions.push_back(i);
        }
    }

    if (possible_directions.size() == 1) {
        direction(possible_directions.at(0));
        return;
    }

    poss_dirs_bubble_sort(distances, possible_directions);

    for (auto const& dir : possible_directions) {
        if (dir != -direction()) {
            direction(dir);
            return;
        }
    }
}

bool Ghost::is_home()
{
    return (position().x > 11 * BOCK_SIZE_24 && position().x < 17 * BOCK_SIZE_24)
        && (position().y > 15 * BOCK_SIZE_24 && position().y < 18 * BOCK_SIZE_24);
}

void Ghost::mod_status(bool status)
{
    status_ = status;
}

void Ghost::update_status(Pac const& pac, bool timed_status)
{
    if (pac.is_energized()) {
        status_ = true;
        return;
    }
    status_ = timed_status;
}

void Ghost::update_facing(Pac const& pac)
{
    if (is_home()) {
        return (direction() == Direction::Down) ? facing(3) : facing(1);
    }

    if (pac.is_energized()) {
        return (!is_alive()) ? facing(direction2facing(direction())) : facing(4);
    }

    facing(direction2facing(direction()));
}

void Ghost::update_speed(Pac const& pac)
{
    if (!is_alive() && speed() != 6) {
        speed(6);
        return;
    }

    if (pac.is_energized()) {
        if (speed() != 1)
            speed(1);
    } else {
        if (speed() != 2)
            speed(2);
    }
}

void Ghost::draw(Pac const& pac, Timer ghost_timer, unsigned short timer_target)
{
    if (pac.is_energized() && is_alive() && !is_home()) {
        body_.setColor(0, 0, 255);
        if (ghost_timer.get_ticks() > timer_target - 2000u) {
            if ((ghost_timer.get_ticks() / 250) % 2 == 1) {
                body_.setColor(255, 255, 255);
                eyes_.setColor(255, 0, 0);
            } else {
                eyes_.setColor(255, 255, 255);
            }
        } else {
            eyes_.setColor(255, 255, 255);
        }
    } else {
        eyes_.setColor(255, 255, 255);
        body_.setColor(color_.r, color_.g, color_.b);
    }

    if (is_alive()) {
        auto clip = &ghost_body_sprite_clips_[current_body_frame_ / GHOST_BODY_FRAMES];
        body_.render(position().x - 4, position().y - 4, 0, clip);
    }
    auto clip = &ghost_eye_sprite_clips_[facing()];
    eyes_.render(position().x - 4, position().y - 4, 0, clip);
    current_body_frame_++;
    if (current_body_frame_ / GHOST_BODY_FRAMES >= GHOST_BODY_FRAMES) {
        current_body_frame_ = 0;
    }
}
