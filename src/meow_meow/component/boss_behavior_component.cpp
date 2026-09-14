#include <meow_meow/component/boss_behavior_component.h>
#include <meow_meow/data/global_data.h>
#include <meow_meow/utils.h>

namespace Meow_Meow {
    Boss_Behavior_Component::Boss_Behavior_Component() {}

    Boss_Behavior_Component::~Boss_Behavior_Component() {}

    const Enemy_Behavior_Config& Boss_Behavior_Component::get_behavior_config(void* global_data) const {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        return data->get_config().get_boss_behavior_config();
    }

    void Boss_Behavior_Component::handle_task(Base_Node* target, float delta_time, void* global_data) {
        Boss_State_Machine_Component* state_machine = get_boss_state_machine_component(target);

        /**
         * @Note: in hook time of skill throwing enemy boss need to flipped to face with enemy is thrown
         */
        if (state_machine != nullptr) {
            if (state_machine->is_channelling_skill_throw_enemy(global_data)) {
                Boss_Skill_Throw_Enemy_Component* skill = Utils::get_component<Boss_Skill_Throw_Enemy_Component>(
                    cast_boss_target(target), Const::BOSS_SKILL_THROW_ENEMY_COMPONENT_NAME
                );
                if (skill != nullptr) {
                    Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
                    const Boss_Skill_Throw_Enemy_Config& skil_config =
                        data->get_config().get_boss_skill_throw_enemy_config();
                    float duration_execute = skill->get_current_countdown();
                    if (duration_execute <= skil_config.duration_hook_enemy) {
                        this->enemy_direction = {-skill->get_hook_enemy_direction(), 1};
                        return;
                    }
                }
            }
        }
        Enemy_Behavior_Component::handle_task(target, delta_time, global_data);
    }

}  // namespace Meow_Meow