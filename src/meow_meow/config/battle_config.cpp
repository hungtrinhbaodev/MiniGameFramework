#include <meow_meow/config/battle_config.h>

#include <iostream>

namespace Meow_Meow {
    Battle_Config::Battle_Config() {}

    Battle_Config::Battle_Config(
        int number_wave,
        std::vector<float> duration_enemies_generated,
        std::vector<int> number_enemies_generated,
        std::vector<int> number_boss_generated
    ) {
        this->number_wave = number_wave;
        this->duration_enemies_generated = duration_enemies_generated;
        this->number_enemies_generated = number_enemies_generated;
        this->number_boss_generated = number_boss_generated;
    }

    Battle_Config::~Battle_Config() {}

    int Battle_Config::get_number_wave() const {
        return this->number_wave;
    }

    float Battle_Config::get_number_enemies_at(int wave) const {
        if (wave >= this->number_enemies_generated.size())
            return 0;

        return this->number_enemies_generated[wave];
    }

    float Battle_Config::get_number_bosses_at(int wave) const {
        if (wave >= this->number_boss_generated.size())
            return 0;

        return this->number_boss_generated[wave];
    }

    float Battle_Config::get_duration_generate_enemy_at(int wave) const {
        if (wave >= this->duration_enemies_generated.size())
            return 0;
        return this->duration_enemies_generated[wave];
    }

    float Battle_Config::get_total_duration_generate() const {
        float total_duration = 0;
        for (int duration : duration_enemies_generated) {
            total_duration += duration;
        }
        return total_duration;
    }

}  // namespace Meow_Meow