#pragma once
#include <meow_meow/config/character_animation_config.h>
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
    };

}  // namespace Meow_Meow