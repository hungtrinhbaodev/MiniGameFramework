#include <animation_node.h>

#include <iostream>

bool Animation_Node::is_valid_animation(std::string name) {
    return animations.find(name) != animations.end();
}

Node_Type Animation_Node::get_type() {
    return Node_Type::ANIMATION;
}

void Animation_Node::make_animation(
    std::string name, std::string folder_path, int number_frame, float duration_loop, std::string extend_format
) {
    if (is_valid_animation(name)) {
        std::cout << "Animation_Node WARNING: animation " << name << " is exists!" << std::endl;
    }
    Animation_Data animation{name, folder_path, number_frame, duration_loop, extend_format};
    animations[name] = animation;
}

void Animation_Node::set_speed(float speed) {
    speed_ratio = std::min(std::max(speed, 0.f), 1.f);
}

float Animation_Node::get_amimation_duration(std::string name) {
    if (!is_valid_animation(name)) {
        return -1.f;
    }
    Animation_Data& animation = animations[current_animation];
    return animation.duration_loop * animation.number_frame;
}

void Animation_Node::clear_all_animation() {
    animations.clear();
}

void Animation_Node::play_animation(std::string name, float speed, bool is_reset) {
    if (!is_valid_animation(name)) {
        std::cout << "Animation_Node WARNING: can't find animation " << name << " to play!" << std::endl;
        return;
    }
    if (current_animation == name && !is_reset) {
        set_speed(speed);
        return;
    }
    current_animation = name;
    current_frame = 0;
    total_delta_time = 0.0f;
    set_speed(speed);
}

void Animation_Node::flex_update(float delta_time) {
    if (!is_valid_animation(current_animation)) {
        set_visible(false);
        return;
    }
    set_visible(true);
    total_delta_time += delta_time;
    Animation_Data& animation = animations[current_animation];
    if (total_delta_time >= animation.duration_loop * speed_ratio) {
        std::string frame_number_str =
            current_frame < 10 ? ("0" + std::to_string(current_frame)) : std::to_string(current_frame);
        std::string current_image = animation.folder_path + frame_number_str + animation.extend_format;

        set_image(current_image);
        total_delta_time = 0;
        current_frame = (current_frame + 1) % animation.number_frame;
    }
}