#pragma once
#include <animation_node.h>

namespace Meow_Meow {
    class Character_Animation : public Animation_Node {
    public:
        Character_Animation();
        Character_Animation(int character_id, int character_level);
        ~Character_Animation();

        void set_character_id(int character_id);
        void set_character_level(int character_level);
        void play_animation(std::string name, float speed = 1.0f);

    protected:
        void fix_update(float delta_time, void* global_data) override;

    private:
        int character_id = -1;
        int character_level = 0;
        bool is_initialize = false;
        /**
         * @Note: because we use fix update to initialize
         * animation so play animation can be call before
         * initialize in this case we cache it and use later!
         */
        std::string waiting_animation = "";
        float waiting_rate = 1.f;

        bool intialize(void* global_data);
    };

}  // namespace Meow_Meow