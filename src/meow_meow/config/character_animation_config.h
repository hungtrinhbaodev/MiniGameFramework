#pragma once
#include <map>
#include <string>
#include <vector>

namespace Meow_Meow {

    struct Character_Animation_Information {
        int character_id;
        std::string default_path;
        std::string animation_name;
        std::string path_animation_folder;

        float duration_per_frame;
        int number_frame;
        int character_max_level;
        int offset_folder;

        std::string get_full_path(int level) const;
    };

    class Character_Animation_Config {
    public:
        Character_Animation_Config();
        Character_Animation_Config(
            int base_chacter_id,
            int number_character,
            std::string default_path,
            std::vector<std::string> animation_names,
            std::vector<std::string> animation_folder_names,
            std::vector<int> animation_number_frames,
            std::vector<int> character_max_levels,
            std::vector<float> animation_duration_per_frames
        );
        ~Character_Animation_Config();

        Character_Animation_Information get_config(int character_id, std::string animation_name) const;
        std::vector<Character_Animation_Information> get_character_animations(int character_id) const;

    private:
        int BASE_CHARACTER_ID;
        std::vector<int> CHARACTER_IDS;
        std::string DEFAULT_PATH;
        std::vector<std::string> ANIMATION_NAMES;
        std::vector<std::string> ANIMATION_FOLDER_NAMES;
        std::vector<int> ANIMATION_NUMBER_FRAMES;
        std::vector<float> ANIMATION_DURATION_PER_FRAMES;
        std::vector<int> CHARACTER_MAX_LEVELS;

        std::vector<Character_Animation_Information> characters;
        void parse_config_to_information();
    };

}  // namespace Meow_Meow