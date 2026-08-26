#include <meow_meow/data/player_data.h>

namespace Meow_Meow {
    Player_Data::Player_Data() {}

    Player_Data::~Player_Data() {}

    int Player_Data::get_character_animation_id() {
        return this->character_animation_id;
    }

    int Player_Data::get_character_level() {
        return this->character_level;
    }

    float Player_Data::get_player_damage() {
        return this->damage;
    }

    float Player_Data::get_current_health() {
        return this->current_health;
    }

    float Player_Data::get_max_health() {
        return this->max_health;
    }

    bool Player_Data::is_dead() {
        return this->dead;
    }

    void Player_Data::set_character_animation_id(int id) {
        this->character_animation_id = id;
    }

    void Player_Data::set_chracter_level(int level) {
        this->character_level = level;
    }

    void Player_Data::set_player_damage(float player_damage) {
        this->damage = player_damage;
    }

    void Player_Data::set_current_health(float current_health) {
        this->current_health = current_health;
    }

    void Player_Data::set_max_health(float max_health) {
        this->max_health = max_health;
    }

    void Player_Data::set_dead(bool dead) {
        this->dead = dead;
    }
}  // namespace Meow_Meow