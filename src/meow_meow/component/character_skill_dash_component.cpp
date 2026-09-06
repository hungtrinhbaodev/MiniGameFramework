#include <meow_meow/component/character_skill_dash_component.h>
#include <meow_meow/config/character_skill_dash_config.h>
#include <meow_meow/const.h>
#include <meow_meow/data/global_data.h>

namespace Meow_Meow {
    Character_Skill_Dash_Component::Character_Skill_Dash_Component() {}

    Character_Skill_Dash_Component::~Character_Skill_Dash_Component() {}

    bool Character_Skill_Dash_Component::can_activate_skill(State_Machine_Component* state_machine, void* global_data) {
        if (state_machine->get_current_state_at(Const::TRACK_EFFECTED) == Const::STATE_ATTACKED ||
            state_machine->get_current_state_at(Const::TRACK_CONTROLL) == Const::STATE_DASHING ||
            state_machine->get_current_state_at(Const::TRACK_CONTROLL) == Const::STATE_SKILL_CHANNELLING) {
            return false;
        }
        return Skill_Component::can_activate_skill(state_machine, global_data);
    }

    void Character_Skill_Dash_Component::attach(Base_Node* target, void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const Character_Skill_Dash_Config& skill_config = data->get_config().get_character_skill_dash_config();
        this->set_skill_id(skill_config.skill_id);
        this->set_countdown_time(skill_config.duration_countdown);
    }

}  // namespace Meow_Meow