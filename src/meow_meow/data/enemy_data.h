#pragma once

namespace Meow_Meow {
    class Enemy_Data {
    public:
        Enemy_Data();
        Enemy_Data(int enemy_id, int enemy_animation_id, float current_health, float max_health, bool dead);
        ~Enemy_Data();

        int get_enemy_id();
        int get_enemy_animation_id();
        float get_current_health();
        float get_max_health();
        bool is_dead();

        void set_enemy_id(int enemy_id);
        void set_animation_id(int enemy_animation_id);
        void set_current_health(float current_health);
        void set_max_health(float max_health);
        void set_dead(bool dead);

    private:
        int enemy_id;
        int enemy_animation_id;
        float current_health;
        float max_health;
        bool dead;
    };
}  // namespace Meow_Meow