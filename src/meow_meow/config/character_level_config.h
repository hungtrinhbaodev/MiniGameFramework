#pragma once
#include <vector>

namespace Meow_Meow {
    class Character_Level_Config {
    public:
        Character_Level_Config();
        Character_Level_Config(
            int number_max_level, std::vector<int> exp_by_levels, std::vector<float> damage_bonus_by_levels
        );
        ~Character_Level_Config();

        int get_number_max_level() const;
        float get_bonus_damage_at_level(int current_level) const;
        int get_exp_next_level(int current_level) const;

    private:
        int number_max_level;
        std::vector<int> exp_by_levels;
        std::vector<float> damage_bonus_by_levels;
    };
}  // namespace Meow_Meow