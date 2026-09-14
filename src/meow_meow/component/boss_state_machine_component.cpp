#include <meow_meow/component/boss_state_michine_component.h>
#include <meow_meow/utils.h>

namespace Meow_Meow {
    Boss_State_Machine_Component::Boss_State_Machine_Component() {}

    Boss_State_Machine_Component::~Boss_State_Machine_Component() {}

    bool Boss_State_Machine_Component::is_channelling_skill_flash(void* global_data) {
        std::string current_state = this->get_current_state_at(Const::TRACK_CONTROLL);
        if (current_state != Const::STATE_SKILL_CHANNELLING) {
            return false;
        }
        if (this->get_source_call_state_at(Const::TRACK_CONTROLL, current_state) !=
            Const::BOSS_CHANNELING_FROM_SKILL_FLASH) {
            return false;
        }
        return true;
    }

    bool Boss_State_Machine_Component::is_channelling_skill_throw_enemy(void* global_data) {
        std::string current_state = this->get_current_state_at(Const::TRACK_CONTROLL);
        if (current_state != Const::STATE_SKILL_CHANNELLING) {
            return false;
        }
        if (this->get_source_call_state_at(Const::TRACK_CONTROLL, current_state) !=
            Const::BOSS_CHANNELING_FROM_SKILL_THROW_ENEMY) {
            return false;
        }
        return true;
    }

    bool Boss_State_Machine_Component::is_using_skill_falsh(void* global_data) {
        return this->is_channelling_skill_flash(global_data) ||
               this->get_current_state_at(Const::TRACK_CONTROLL) == Const::STATE_SKILL_FLASH;
    }

    bool Boss_State_Machine_Component::is_using_skill_throw_enemy(void* global_data) {
        return this->is_channelling_skill_throw_enemy(global_data);
    }

    bool Boss_State_Machine_Component::is_using_some_skill(void* global_data) {
        return this->is_using_skill_falsh(global_data) || this->is_using_skill_throw_enemy(global_data);
    }

    bool Boss_State_Machine_Component::can_take_bullet_damage(void* global_data) {
        if (this->is_channelling_skill_flash(global_data) || this->is_using_skill_falsh(global_data)) {
            return false;
        }
        return Enemy_State_Machine_Component::can_take_bullet_damage(global_data);
    }

    void Boss_State_Machine_Component::handle_auto_change_state(
        Base_Node* target, std::string track, std::string state, void* global_data, int source_call_state
    ) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);

        if (track == Const::TRACK_CONTROLL) {
            if (state == Const::STATE_SKILL_CHANNELLING) {
                int source_call_tag =
                    this->get_source_call_state_at(Const::TRACK_CONTROLL, Const::STATE_SKILL_CHANNELLING);
                switch (source_call_tag) {
                    case Const::BOSS_CHANNELING_FROM_SKILL_FLASH: {
                        const Boss_Skill_Flash_Config& flash_skill_config =
                            data->get_config().get_boss_skill_flash_config();
                        this->change_state_at(
                            Const::TRACK_CONTROLL, Const::STATE_SKILL_FLASH, flash_skill_config.duration_flash
                        );
                        return;
                    }
                    case Const::BOSS_CHANNELING_FROM_SKILL_THROW_ENEMY: {
                        Boss_Node* boss = cast_boss_target(target);
                        const Enemy_Behavior_Config& behavior = boss->get_behavior_config_from(global_data);
                        this->change_state_at(
                            Const::TRACK_CONTROLL, Const::STATE_MOVE, behavior.get_enemy_walk_duration()
                        );
                        return;
                    }
                }
            } else if (state == Const::STATE_SKILL_FLASH) {
                Boss_Node* boss_node = cast_boss_target(target);
                Enemy_Behavior_Component* boss_behavior = get_enemy_bihavior_component(target);
                const Enemy_Behavior_Config& behavior_config = boss_node->get_behavior_config_from(global_data);

                if (boss_behavior == nullptr) {
                    this->change_state_at(
                        Const::TRACK_CONTROLL, Const::STATE_MOVE, behavior_config.get_enemy_walk_duration()
                    );
                    return;
                }
                if (boss_behavior->can_attack()) {
                    this->change_state_at(
                        Const::TRACK_CONTROLL, Const::STATE_ATTACK, behavior_config.get_enemy_attacked_duration()
                    );
                    return;
                }
                this->change_state_at(
                    Const::TRACK_CONTROLL, Const::STATE_MOVE, behavior_config.get_enemy_attacked_duration()
                );
            }
        }

        Enemy_State_Machine_Component::handle_auto_change_state(target, track, state, global_data, source_call_state);
    }
}  // namespace Meow_Meow