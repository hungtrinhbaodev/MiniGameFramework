#pragma once
#include <label_node.h>
#include <node.h>
#include <progression_node.h>

#include <string>

namespace Meow_Meow {
    class Character_Skill_UI : public Node {
    public:
        Character_Skill_UI();
        Character_Skill_UI(std::string skill_id);
        ~Character_Skill_UI();

        float get_ui_height();

    protected:
        void attach(void* global_data) override;
        void fix_update(float delta_time, void* global_data) override;

    private:
        const int LABEL_SKILL_FONT_SIZE = 22;
        const Custom::Color LABEL_SKILL_COLOR = {230, 230, 230};
        const Custom::Color INNER_PROGRESSION_COLOR = {30, 30, 30};
        const unsigned char INNER_PROGRESSION_OPACITY = 160;
        const glm::vec2 ORIGIN_PRGRESSION_SCALE = {0.65f, 0.65f};
        const float PROGRESSION_FIX_WIDTH = 109 * ORIGIN_PRGRESSION_SCALE.x;
        const float PROGRESSION_FIX_HEIGHT = 97 * ORIGIN_PRGRESSION_SCALE.y;
        const float GAP_WIDTH_UI = 5.f;

        void init_progression_skill(const Skill_Information* skill);
        void init_label_skill(const Skill_Information* skill);

        std::string skill_id = "";
        Progression_Node* progression_skill = nullptr;
        Label_Node* label_skill_name = nullptr;
    };
}  // namespace Meow_Meow