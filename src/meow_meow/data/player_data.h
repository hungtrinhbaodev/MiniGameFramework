#pragma once

namespace Meow_Meow {
    class Player_Data {
    public:
        Player_Data();
        ~Player_Data();

        int get_character_animation_id();
        int get_character_level();
        float get_player_damage();
        float get_current_health();
        float get_max_health();
        bool is_dead();
        int get_current_exp();

        void set_character_animation_id(int animation_id);
        void set_chracter_level(int level);
        void set_player_damage(float player_damage);
        void set_current_health(float current_health);
        void set_max_health(float max_health);
        void set_dead(bool dead);
        void set_current_exp(int current_exp);

    private:
        int character_animation_id = 2;
        int character_level = 1;

        float damage = 0.f;
        float current_health = 0.f;
        float max_health = 0.f;
        int exp = 0;

        bool dead = false;
    };
}  // namespace Meow_Meow