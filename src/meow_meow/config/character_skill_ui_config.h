#pragma once
#include <string>
#include <vector>

namespace Meow_Meow {
    struct Skill_Information {
        std::string skill_id = "";
        std::string skill_display_name = "";
        std::string skill_image_path = "";
    };

    struct Character_Skill_UI_Config {
        std::vector<Skill_Information> skills;

        Character_Skill_UI_Config() {}

        Character_Skill_UI_Config(
            std::vector<std::string> skill_ids,
            std::vector<std::string> skill_display_names,
            std::vector<std::string> skill_image_paths
        ) {
            for (int i = 0; i < skill_ids.size(); i++) {
                skills.push_back({skill_ids[i], skill_display_names[i], skill_image_paths[i]});
            }
        }

        const Skill_Information* find_skill(std::string skill_id) const {
            for (int i = 0; i < this->skills.size(); i++) {
                if (this->skills[i].skill_id == skill_id) {
                    return &this->skills[i];
                }
            }
            return nullptr;
        }
    };
}  // namespace Meow_Meow