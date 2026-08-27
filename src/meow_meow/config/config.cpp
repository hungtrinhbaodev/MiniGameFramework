#include <meow_meow/config/config.h>

namespace Meow_Meow {
    Config::Config() {
        this->battle_levels.push_back(
            {Const::BATTLE_NUMBER_WAVE, Const::BATTLE_DURATION_GENERATE_ENEMY, Const::BATTLE_NUMBER_ENEMY_GENEERATED}
        );
    }

    Config::~Config() {}

    bool Config::is_character_id(int character_id) const {
        return character_id >= Const::BASE_CHARACTER_ID &&
               character_id < Const::BASE_CHARACTER_ID + Const::NUMBER_CHARACTER;
    }

    const Character_Animation_Config& Config::get_character_animation_config() const {
        return this->character_animation_config;
    }

    const Character_Animation_Config& Config::get_enemy_animation_config() const {
        return this->enemy_animation_config;
    }

    const Enemy_Behavior_Config& Config::get_enemy_behavior_config() const {
        return this->enemy_behavior_config;
    }

    const Character_Behavior_Config& Config::get_character_behavior_config() const {
        return this->character_behavior_config;
    }

    const Bullet_Behavior_Config& Config::get_bullet_behavior_config() const {
        return this->bullet_behavior_config;
    }

    const Battle_Config& Config::get_battle_config_at(int level) const {
        if (level >= this->battle_levels.size()) {
            return this->default_battle_config;
        }
        return this->battle_levels[level];
    }

    std::string Config::get_character_name(int character_id) const {
        if (!this->is_character_id(character_id)) {
            return "";
        }
        for (auto& character_info : Const::CHARACTERS_INFO) {
            if (character_info.id == character_id) {
                return character_info.name;
            }
        }
        return "";
    }

    std::vector<int> Config::get_character_ids() const {
        std::vector<int> ids{};
        for (auto& character_info : Const::CHARACTERS_INFO) {
            ids.push_back(character_info.id);
        }
        return ids;
    }

}  // namespace Meow_Meow