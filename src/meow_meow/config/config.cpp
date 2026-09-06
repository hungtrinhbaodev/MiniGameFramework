#include <meow_meow/config/config.h>

namespace Meow_Meow {
    Config::Config() {
        this->battle_levels.push_back(
            {Const::BATTLE_NUMBER_WAVE,
             Const::BATTLE_DURATION_GENERATE_ENEMY,
             Const::BATTLE_NUMBER_ENEMY_GENERATED,
             Const::BATTLE_NUMBER_BOSS_GENERATED}
        );
    }

    Config::~Config() {}

    bool Config::is_character_animation_id(int character_id) const {
        return character_id >= Const::BASE_CHARACTER_ID &&
               character_id < Const::BASE_CHARACTER_ID + Const::NUMBER_CHARACTER;
    }

    bool Config::is_boss_animation_id(int character_id) const {
        return character_id >= Const::BASE_BOSS_ID && character_id < Const::BASE_BOSS_ID + Const::NUMBER_ANIMATION_BOSS;
    }

    bool Config::is_boss_flash_skill(std::string skill_id) const {
        const Boss_Skill_Flash_Config& skill_config = this->get_boss_skill_flash_config();
        return skill_config.skill_id == skill_id;
    }

    const Character_Animation_Config& Config::get_character_animation_config() const {
        return this->character_animation_config;
    }

    const Character_Animation_Config& Config::get_enemy_animation_config() const {
        return this->enemy_animation_config;
    }

    const Character_Animation_Config& Config::get_boss_animation_config() const {
        return this->boss_animation_config;
    }

    const Enemy_Behavior_Config& Config::get_enemy_behavior_config() const {
        return this->enemy_behavior_config;
    }

    const Enemy_Behavior_Config& Config::get_boss_behavior_config() const {
        return this->boss_behavior_config;
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

    const Character_Level_Config& Config::get_character_level_config() const {
        return this->character_level_config;
    }

    const Character_Skill_Dash_Config& Config::get_character_skill_dash_config() const {
        return this->character_skill_dash_config;
    }

    const Character_Skill_Thunder_Config& Config::get_character_skill_thunder_config() const {
        return this->character_skill_thunder_config;
    }

    const Enemy_Skill_Jump_Config& Config::get_enemy_skill_jump_config() const {
        return this->enemy_skil_jump_config;
    }

    const Boss_Skill_Flash_Config& Config::get_boss_skill_flash_config() const {
        return this->boss_skil_flash_config;
    }

    std::string Config::get_character_name(int character_id) const {
        if (!this->is_character_animation_id(character_id)) {
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