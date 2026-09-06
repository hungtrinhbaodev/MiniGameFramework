#pragma once
#include <vector>

namespace Meow_Meow {
    class Battle_Config {
    public:
        Battle_Config();
        Battle_Config(
            int number_wave,
            std::vector<float> duration_enemies_generated,
            std::vector<int> number_enemies_generated,
            std::vector<int> number_boss_generated
        );
        ~Battle_Config();

        int get_number_wave() const;
        float get_number_enemies_at(int wave) const;
        float get_number_bosses_at(int wave) const;
        float get_duration_generate_enemy_at(int wave) const;

    private:
        int number_wave = 0;
        std::vector<float> duration_enemies_generated;
        std::vector<int> number_enemies_generated;
        std::vector<int> number_boss_generated;
    };
}  // namespace Meow_Meow