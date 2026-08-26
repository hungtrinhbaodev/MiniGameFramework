#pragma once
#include <meow_meow/config/bullet_behavior_config.h>
#include <meow_meow/config/character_animation_config.h>
#include <meow_meow/config/character_behavior_config.h>
#include <meow_meow/config/enemy_behavior_config.h>
#include <meow_meow/const.h>

#include <string>

namespace Meow_Meow {

    class Config {
    public:
        Config();
        ~Config();

        bool is_character_id(int character_id) const;
        const Character_Animation_Config& get_character_animation_config() const;
        const Character_Animation_Config& get_enemy_animation_config() const;
        const Enemy_Behavior_Config& get_enemy_behavior_config() const;
        const Character_Behavior_Config& get_character_behavior_config() const;
        const Bullet_Behavior_Config& get_bullet_behavior_config() const;
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
            Const::NUMBER_ENEMY,
            Const::ENEMY_DEFAULT_PATH,
            Const::ENEMY_ANIMATION_NAMES,
            Const::ENEMY_ANIMATION_FOLDER_NAMES,
            Const::ENEMY_ANIMATION_NUMBER_FRAMES,
            Const::ENEMY_MAX_LEVELS,
            Const::ENEMY_ANIMATION_DURATION_PER_FRAMES
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
            Const::ENEMY_BOUNDING_BOX
        };
        Bullet_Behavior_Config bullet_behavior_config{
            Const::BULLET_VELOSITY, Const::BULLET_ACCELARATE, Const::BULLET_BOUNDING_BOX
        };
    };

}  // namespace Meow_Meow