#pragma once
#include <meow_meow/config/battle_config.h>
#include <meow_meow/config/boss_skill_flash_config.h>
#include <meow_meow/config/bullet_behavior_config.h>
#include <meow_meow/config/character_animation_config.h>
#include <meow_meow/config/character_behavior_config.h>
#include <meow_meow/config/character_level_config.h>
#include <meow_meow/config/character_skill_dash_config.h>
#include <meow_meow/config/character_skill_thunder_config.h>
#include <meow_meow/config/enemy_behavior_config.h>
#include <meow_meow/config/enemy_skill_jump_config.h>
#include <meow_meow/const.h>

#include <string>

namespace Meow_Meow {

    class Config {
    public:
        Config();
        ~Config();

        bool is_character_animation_id(int character_id) const;
        bool is_boss_animation_id(int character_id) const;
        bool is_boss_flash_skill(std::string skill_id) const;
        const Character_Animation_Config& get_character_animation_config() const;
        const Character_Animation_Config& get_enemy_animation_config() const;
        const Character_Animation_Config& get_boss_animation_config() const;
        const Enemy_Behavior_Config& get_enemy_behavior_config() const;
        const Enemy_Behavior_Config& get_boss_behavior_config() const;
        const Character_Behavior_Config& get_character_behavior_config() const;
        const Bullet_Behavior_Config& get_bullet_behavior_config() const;
        const Battle_Config& get_battle_config_at(int level) const;
        const Character_Level_Config& get_character_level_config() const;
        const Character_Skill_Dash_Config& get_character_skill_dash_config() const;
        const Character_Skill_Thunder_Config& get_character_skill_thunder_config() const;
        const Enemy_Skill_Jump_Config& get_enemy_skill_jump_config() const;
        const Boss_Skill_Flash_Config& get_boss_skill_flash_config() const;
        std::string get_character_name(int character_id) const;
        std::vector<int> get_character_ids() const;

    private:
        Character_Animation_Config character_animation_config{
            Const::BASE_CHARACTER_ID,
            Const::NUMBER_CHARACTER,
            Const::CHARACTER_DEFAULT_PATH,
            Const::CHARACTER_ANIMATION_NAMES,
            Const::CHARACTER_ANIMATION_FOLDER_NAMES,
            Const::CHARACTER_ANIMATION_NUMBER_FRAMES,
            Const::CHARACTER_MAX_LEVELS,
            Const::CHARACTER_ANIMATION_DURATION_PER_FRAMES
        };
        Character_Animation_Config enemy_animation_config{
            Const::BASE_ENEMY_ID,
            Const::NUMBER_ANIMATION_ENEMY,
            Const::ENEMY_DEFAULT_PATH,
            Const::ENEMY_ANIMATION_NAMES,
            Const::ENEMY_ANIMATION_FOLDER_NAMES,
            Const::ENEMY_ANIMATION_NUMBER_FRAMES,
            Const::ENEMY_MAX_LEVELS,
            Const::ENEMY_ANIMATION_DURATION_PER_FRAMES
        };
        Character_Animation_Config boss_animation_config{
            Const::BASE_BOSS_ID,
            Const::NUMBER_ANIMATION_BOSS,
            Const::BOSS_DEFAULT_PATH,
            Const::ENEMY_ANIMATION_NAMES,
            Const::ENEMY_ANIMATION_FOLDER_NAMES,
            Const::BOSS_ANIMATION_NUMBER_FRAMES,
            Const::ENEMY_MAX_LEVELS,
            Const::BOSS_ANIMATION_DURATION_PER_FRAMES
        };
        Character_Behavior_Config character_behavior_config{
            Const::CHARACTER_ATTACKED_DURATION,
            Const::CHARACTER_INVINCIBLE_DURATION,
            Const::CHARACTER_VELOSITY,
            Const::CHARACTER_ACCELARATE,
            Const::CHARACTER_ATTACK_DURATION,
            Const::CHARACTER_BONUS_VELOSITY_RATE,
            Const::CHARACTER_BONUS_VELOSITY,
            Const::CHARACTER_MAX_BONUS_VELOSITY,
            Const::CHARACTER_BOUNDING_BOX,
            Const::CHARACTER_HEALTH,
            Const::CHARACTER_DAMAGE,
            Const::CHARACTER_DEAD_DURATION
        };
        Enemy_Behavior_Config enemy_behavior_config{
            Const::ENEMY_JUMP_DISTANCE,
            Const::ENEMY_VELOSITY,
            Const::ENEMY_ATTACK_DAMGE,
            Const::ENEMY_ATTACK_RANGE,
            Const::ENEMY_DURATION_WALK,
            Const::ENEMY_DURATION_JUMP,
            Const::ENEMY_DURATION_ATTACK,
            Const::ENEMY_ATTACK_COUNTDOWN,
            Const::ENEMY_JUMP_COUNTDOWN,
            Const::ENEMY_ATTACKED_DURATION,
            Const::ENEMY_ATTACKED_VELOSITY,
            Const::ENEMY_HEALTH,
            Const::ENEMY_BOUNDING_BOX,
            Const::ENEMY_KILLED_EXP,
            Const::ENEMY_ENABLE_JUMP
        };
        Enemy_Behavior_Config boss_behavior_config{
            Const::ENEMY_JUMP_DISTANCE,
            Const::BOSS_VELOSITY,
            Const::BOSS_ATTACK_DAMGE,
            Const::ENEMY_ATTACK_RANGE,
            Const::ENEMY_DURATION_WALK,
            Const::ENEMY_DURATION_JUMP,
            Const::BOSS_DURATION_ATTACK,
            Const::ENEMY_ATTACK_COUNTDOWN,
            Const::ENEMY_JUMP_COUNTDOWN,
            Const::ENEMY_ATTACKED_DURATION,
            Const::ENEMY_ATTACKED_VELOSITY,
            Const::BOSS_HEALTH,
            Const::BOSS_BOUNDING_BOX,
            Const::ENEMY_KILLED_EXP,
            Const::BOSS_ENABLE_JUMP
        };
        Bullet_Behavior_Config bullet_behavior_config{
            Const::BULLET_VELOSITY, Const::BULLET_ACCELARATE, Const::BULLET_BOUNDING_BOX
        };
        Character_Level_Config character_level_config{
            Const::CHARACTER_MAX_LEVEL_NUMBER, Const::EXP_AT_LEVELS, Const::DAMAGE_BONUS_AT_LEVELS
        };
        Character_Skill_Dash_Config character_skill_dash_config{
            Const::CHARACTER_SKILL_DASH_COUNTDOWN,
            Const::CHARACTER_SKILL_DASH_ID,
            Const::CHARACTER_SKILL_DASH_DURATION,
            Const::CHARACTER_SKILL_DASH_DISTANCE
        };
        Character_Skill_Thunder_Config character_skill_thunder_config{
            Const::CHARACTER_SKILL_THUNDER_ID,
            Const::CHARACTER_SKILL_THUNDER_COUNTDOWN,
            Const::CHARACTER_SKILL_THUNDER_NUMBER_TAKEN,
            Const::CHARACTER_SKILL_THUNDER_CHANNELLING_DURATION,
            Const::CHARACTER_SKILL_THUNDER_TAKEN_RADIUS,
            Const::CHARACTER_SKILL_THUNDER_DAMAGE_TAKEN,
            Const::CHARACTER_SKILL_THUNDER_DURATION_STUN
        };
        Enemy_Skill_Jump_Config enemy_skil_jump_config{
            Const::ENEMY_SKILL_JUMP_ID,
            Const::ENEMY_SKILL_JUMP_COUNTDOWN,
            Const::ENEMY_SKILL_JUMP_DISTANCE,
            Const::ENEMY_SKILL_JUMP_DURATION
        };
        Boss_Skill_Flash_Config boss_skil_flash_config{
            Const::BOSS_SKILL_FLASH_ID,
            Const::BOSS_SKILL_FLASH_COUNTDOWN,
            Const::BOSS_SKILL_FLASH_DURATION,
            Const::BOSS_SKILL_FLASH_CHANNELLING_DURATION,
            Const::BOSS_SKILL_FLASH_DAMAGE,
            Const::BOSS_SKILL_FLASH_DISTANCE,
            Const::BOSS_SKILL_FLASH_DURATION_FLY,
            Const::BOSS_SKILL_FLASH_DURATION_STUN
        };
        Battle_Config default_battle_config{};
        std::vector<Battle_Config> battle_levels;
    };

}  // namespace Meow_Meow