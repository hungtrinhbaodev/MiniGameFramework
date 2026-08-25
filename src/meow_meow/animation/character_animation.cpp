#include <meow_meow/animation/character_animation.h>
#include <meow_meow/data/global_data.h>

namespace Meow_Meow {

    Character_Animation::Character_Animation() {}

    Character_Animation::Character_Animation(int character_id, int character_level) {
        this->set_character_id(character_id);
        this->set_character_level(character_level);
        this->set_name("Character_Animation");
    }

    Character_Animation::~Character_Animation() {}

    void Character_Animation::set_character_id(int character_id) {
        this->character_id = character_id;
        this->is_initialize = false;
    }

    void Character_Animation ::set_character_level(int character_level) {
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
        Animation_Node::play_animation(name, speed, is_reset);
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
        auto character_config = config.get_enemy_animation_config();
        if (config.is_character_id(this->character_id)) {
            character_config = config.get_character_animation_config();
        }
        std::vector<Character_Animation_Information> animations =
            character_config.get_character_animations(this->character_id);
        for (const auto& animation : animations) {
            this->make_animation(
                animation.animation_name,
                animation.get_full_path(this->character_level),
                animation.number_frame,
                animation.duration_per_frame,
                ".png"
            );
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