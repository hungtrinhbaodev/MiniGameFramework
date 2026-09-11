#include <actions.h>
#include <defined.h>
#include <math_custom.h>
#include <meow_meow/animation/character_animation.h>
#include <meow_meow/data/global_data.h>

namespace Meow_Meow {

    Character_Animation::Character_Animation() {}

    Character_Animation::Character_Animation(int character_id, int character_level) {
        this->set_character_id(character_id);
        this->set_character_level(character_level);
    }

    Character_Animation::~Character_Animation() {}

    void Character_Animation::set_character_id(int character_id) {
        this->character_id = character_id;
        this->is_initialize = false;
    }

    void Character_Animation::set_character_level(int character_level) {
        this->character_level = character_level;
        this->is_initialize = false;
    }

    void Character_Animation::play_animation(std::string name, float speed, bool is_reset) {
        if (!this->is_initialize) {
            this->waiting_animation = name;
            this->waiting_rate = speed;
            this->waiiting_is_reset = is_reset;
            return;
        }
        if (name == "DEAD" && !this->is_load_all_smooth_frame(name)) {
            float duration = this->get_amimation_duration(name) * speed * 0.75;
            float sign = this->is_flipped_x() ? -1 : 1;
            this->do_action(
                Action::sequence(
                    Action::spawn(
                        Action::rotate_to(duration * 0.65, sign * 90, Action_Ease::SINE_IN),
                        Action::rotate_by(duration, sign * 360, Action_Ease::SINE_IN),
                        Action::sequence(
                            Action::move_to(
                                duration * 0.5,
                                this->get_position() +
                                    glm::vec2(sign * Math::random_float(50, 100), 150 + Math::random_float(0, 50)),
                                Action_Ease::SINE_OUT
                            ),
                            Action::move_to(duration / 2, this->get_position(), Action_Ease::SINE_IN)
                        ),
                        Action::sequence(
                            Action::scale_to(
                                duration / 2, glm::vec2{1.1f, 1.1f} * this->get_scale(), Action_Ease::SINE_OUT
                            ),
                            Action::scale_to(
                                duration / 2, glm::vec2{0.85f, 0.85f} * this->get_scale(), Action_Ease::SINE_IN
                            )
                        ),
                        Action::sequence(Action::delay(duration * 0.5), Action::fade_out(duration * 0.75))
                    ),
                    Action::hide()
                )
            );
        }
        Animation_Node::play_animation(name, speed, is_reset);
    }

    const Character_Animation_Config& Character_Animation::get_animation_config(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const auto& config = data->get_config();
        if (config.is_character_animation_id(this->character_id)) {
            return config.get_character_animation_config();
        }
        if (config.is_boss_animation_id(this->character_id)) {
            return config.get_boss_animation_config();
        }
        return config.get_enemy_animation_config();
    }

    bool Character_Animation::intialize(void* global_data) {
        if (this->character_id < 0) {
            return false;
        }
        if (this->character_level < 1) {
            return false;
        }
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        auto config = data->get_config();
        const auto& character_config = this->get_animation_config(global_data);
        Animation_Node::ANIMATION_LOAD_MODE load_mode = !config.is_character_animation_id(this->character_id)
                                                            ? Animation_Node::ANIMATION_LOAD_MODE::SMOOTH
                                                            : Animation_Node::ANIMATION_LOAD_MODE::IMMEDIATE;
        std::vector<Character_Animation_Information> animations =
            character_config.get_character_animations(this->character_id);
        for (const auto& animation : animations) {
            bool is_dead = animation.animation_name == "DEAD";
            bool is_boss_attack =
                animation.animation_name == "ATTACK" && data->get_config().is_boss_animation_id(this->character_id);
            this->make_animation(
                animation.animation_name,
                animation.get_full_path(this->character_level),
                animation.number_frame,
                animation.duration_per_frame,
                load_mode
            );
            if (is_dead || is_boss_attack) {
                this->set_preload_animation(animation.animation_name);
            }
        }
        return true;
    }

    void Character_Animation::fix_update(float delta_time, void* global_data) {
        if (global_data == nullptr)
            return;

        if (!this->is_initialize) {
            this->is_initialize = this->intialize(global_data);
        }

        if (this->waiting_animation != "") {
            this->play_animation(this->waiting_animation, this->waiting_rate);
            this->waiting_animation = "";
            this->waiting_rate = 1.f;
            this->waiiting_is_reset = false;
        }
    }

    void Character_Animation::draw(Custom::Transform& world_transform, int& draw_index) {
        Image_Node::draw(world_transform, draw_index);
    }

}  // namespace Meow_Meow