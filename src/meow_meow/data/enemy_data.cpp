#include <meow_meow/data/enemy_data.h>

namespace Meow_Meow {
    Enemy_Data::Enemy_Data() {}

    Enemy_Data::Enemy_Data(int enemy_id, int enemy_animation_id, float current_health, float max_health, bool dead) {
        this->enemy_id = enemy_id;
        this->enemy_animation_id = enemy_animation_id;
        this->current_health = current_health;
        this->max_health = max_health;
        this->dead = dead;
    }

    Enemy_Data::~Enemy_Data() {}

    int Enemy_Data::get_enemy_id() {
        return this->enemy_id;
    }

    int Enemy_Data::get_enemy_animation_id() {
        return this->enemy_animation_id;
    }

    float Enemy_Data::get_current_health() {
        return this->current_health;
    }

    float Enemy_Data::get_max_health() {
        return this->max_health;
    }

    bool Enemy_Data::is_dead() {
        return this->dead;
    }

    void Enemy_Data::set_enemy_id(int enemy_id) {
        this->enemy_id = enemy_id;
    }

    void Enemy_Data::set_animation_id(int enemy_animation_id) {
        this->enemy_animation_id = enemy_animation_id;
    }

    void Enemy_Data::set_current_health(float current_health) {
        this->current_health = current_health;
    }

    void Enemy_Data::set_max_health(float max_health) {
        this->max_health = max_health;
    }

    void Enemy_Data::set_dead(bool dead) {
        this->dead = dead;
    }

}  // namespace Meow_Meow