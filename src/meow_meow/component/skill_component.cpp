#include <meow_meow/component/skill_component.h>

namespace Meow_Meow {
    Skill_Component::Skill_Component() {}

    Skill_Component::~Skill_Component() {}

    void Skill_Component::set_skill_id(std::string skill_id) {
        this->skill_id = skill_id;
    }

    void Skill_Component::set_countdown_time(float countdown_time) {
        this->current_countdown_time = countdown_time;
        this->countdown_time = countdown_time;
    }

    void Skill_Component::start_countdown_time() {
        this->current_countdown_time = 0.f;
    }

    std::string Skill_Component::get_skill_id() {
        return this->skill_id;
    }

    bool Skill_Component::can_activate_skill(State_Machine_Component* state_machine, void* global_data) {
        return this->current_countdown_time >= this->countdown_time;
    }

    void Skill_Component::activating_skill(void* global_data) {
        this->start_countdown_time();
    }

    void Skill_Component::update_information(Base_Node* target, float delta_time, void* global_data) {
        if (this->current_countdown_time < this->countdown_time) {
            this->current_countdown_time += delta_time;
        }
    }
}  // namespace Meow_Meow