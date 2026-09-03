#include <meow_meow/config/character_level_config.h>

#include <iostream>

namespace Meow_Meow {
    Character_Level_Config::Character_Level_Config() {}

    Character_Level_Config::Character_Level_Config(
        int number_max_level, std::vector<int> exp_by_levels, std::vector<float> damage_bonus_by_levels
    ) {
        this->number_max_level = number_max_level;
        this->exp_by_levels = exp_by_levels;
        this->damage_bonus_by_levels = damage_bonus_by_levels;
    }

    Character_Level_Config::~Character_Level_Config() {}

    int Character_Level_Config::get_number_max_level() const {
        return this->number_max_level;
    }

    float Character_Level_Config::get_bonus_damage_at_level(int current_level) const {
        current_level -= 1;
        if (current_level >= this->number_max_level || current_level < 0) {
            return -1;
        }
        return this->damage_bonus_by_levels[current_level];
    }

    int Character_Level_Config::get_exp_next_level(int current_level) const {
        if (current_level >= this->number_max_level || current_level < 0) {
            return -1;
        }
        return this->exp_by_levels[current_level];
    }

}  // namespace Meow_Meow