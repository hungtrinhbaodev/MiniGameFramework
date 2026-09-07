#include <layer_node.h>
#include <meow_meow/ui/battle_character_stats_ui.h>
#include <meow_meow/ui/character_skill_ui.h>

namespace Meow_Meow {
    class Layer_Stats : public Layer_Node {
    public:
        Layer_Stats();
        ~Layer_Stats();

    protected:
        void attach(void* global_data) override;
        void fix_update(float delta_time, void* global_data) override;

    private:
        const glm::vec2 ORIGIN_START_SKILL_UI_POSITION = {5.f, 17.f};
        const float GAP_HEIGHT_PER_SKILL_UI = 2.f;

        void init_stats_ui();
        void init_skills_ui(void* global_data);
        void update_skills_ui_location();

        Battle_Character_Stats_UI* stats_ui = nullptr;
        std::vector<Character_Skill_UI*> skills_ui;
    };
}  // namespace Meow_Meow