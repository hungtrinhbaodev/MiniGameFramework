#include <meow_meow/config/character_animation_config.h>
namespace Meow_Meow {

    std::string Character_Animation_Information::get_full_path(int level) const {
        level = std::max(0, std::min(level, this->character_max_level));
        std::string folder_animation = std::to_string(this->offset_folder + level);
        return default_path + folder_animation + "/" + path_animation_folder + "/";
    }

    Character_Animation_Config::Character_Animation_Config(
        int base_chacter_id,
        int number_character,
        std::string default_path,
        std::vector<std::string> animation_names,
        std::vector<std::string> animation_folder_names,
        std::vector<int> animation_number_frames,
        std::vector<int> character_max_levels,
        std::vector<float> animation_duration_per_frames
    ) {
        BASE_CHARACTER_ID = base_chacter_id;
        CHARACTER_IDS.clear();
        for (int i = 0; i < number_character; i++) {
            CHARACTER_IDS.push_back(base_chacter_id + i);
        }
        DEFAULT_PATH = default_path;
        ANIMATION_NAMES = animation_names;
        ANIMATION_FOLDER_NAMES = animation_folder_names;
        ANIMATION_NUMBER_FRAMES = animation_number_frames;
        CHARACTER_MAX_LEVELS = character_max_levels;
        ANIMATION_DURATION_PER_FRAMES = animation_duration_per_frames;
        parse_config_to_information();
    }

    Character_Animation_Config::Character_Animation_Config() {}

    Character_Animation_Config::~Character_Animation_Config() {}

    void Character_Animation_Config::parse_config_to_information() {
        int offset = 0;
        for (int character_id : CHARACTER_IDS) {
            int character_max_level = CHARACTER_MAX_LEVELS[character_id - BASE_CHARACTER_ID];
            for (int i = 0; i < ANIMATION_NAMES.size(); i++) {
                std::string animation_name = ANIMATION_NAMES[i];
                std::string animation_folder_name = ANIMATION_FOLDER_NAMES[i];
                int animation_number_frame = ANIMATION_NUMBER_FRAMES[i];
                float animation_duration_per_frame = ANIMATION_DURATION_PER_FRAMES[i];
                Character_Animation_Information character{
                    character_id,
                    DEFAULT_PATH,
                    animation_name,
                    animation_folder_name,
                    animation_duration_per_frame,
                    animation_number_frame,
                    character_max_level,
                    offset
                };
                this->characters.push_back(character);
            }
            offset += character_max_level;
        }
    }

    Character_Animation_Information Character_Animation_Config::get_config(
        int character_id, std::string animation_name
    ) const {
        for (int i = 0; i < characters.size(); i++) {
            auto character = characters[i];
            if (character.character_id == character_id && character.animation_name == animation_name) {
                return character;
            }
        }
        if (characters.size() > 0) {
            return characters.front();
        }
        return {};
    }

    std::vector<Character_Animation_Information> Character_Animation_Config::get_character_animations(
        int character_id
    ) const {
        std::vector<Character_Animation_Information> animations;
        for (int i = 0; i < characters.size(); i++) {
            auto character = characters[i];
            if (character.character_id == character_id) {
                animations.push_back(character);
            }
        }
        return animations;
    }

}  // namespace Meow_Meow