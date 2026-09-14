#include <meow_meow/component/enemy_state_machine_component.h>
#include <meow_meow/const.h>
#include <meow_meow/data/global_data.h>
#include <meow_meow/utils.h>

namespace Meow_Meow {
    Enemy_State_Machine_Component::Enemy_State_Machine_Component() {}

    Enemy_State_Machine_Component::~Enemy_State_Machine_Component() {}

    bool Enemy_State_Machine_Component::can_take_bullet_damage(void* global_data) {
        if (this->is_enemy_attacked() || this->is_enemy_dead() || this->is_enemy_lost_all_health()) {
            return false;
        }
        return true;
    }

    bool Enemy_State_Machine_Component::is_enemy_dead() {
        return this->get_current_state_at(Const::TRACK_CONTROLL) == Const::STATE_DEATH;
    }

    bool Enemy_State_Machine_Component::is_enemy_attacked() {
        return this->get_current_state_at(Const::TRACK_EFFECTED) == Const::STATE_ATTACKED;
    }

    bool Enemy_State_Machine_Component::is_enemy_lost_all_health() {
        if (this->global_data == nullptr || this->target == nullptr) {
            return false;
        }
        Global_Data* data = reinterpret_cast<Global_Data*>(this->global_data);
        Enemy_Node* enemy = cast_enemy_target(this->target);
        Enemy_Data& enemy_data = enemy->get_enemy_data(this->global_data);
        if (this->get_current_state_at(Const::TRACK_EFFECTED) == Const::STATE_STUN ||
            this->get_current_state_at(Const::TRACK_EFFECTED) == Const::STATE_ATTACKED) {
            return enemy_data.get_current_health() <= 0;
        }
        return false;
    }

    bool Enemy_State_Machine_Component::is_enemy_hooked() {
        if (this->get_current_state_at(Const::TRACK_CONTROLL) == Const::STATE_SKILL_CHANNELLING) {
            if (this->get_source_call_state_at(Const::TRACK_CONTROLL, Const::STATE_SKILL_CHANNELLING) ==
                Const::ENEMY_CHANNLING_BY_SKILL_THROWING) {
                return true;
            }
        }
        return false;
    }

    bool Enemy_State_Machine_Component::is_enemy_jump() {
        return this->get_current_state_at(Const::TRACK_CONTROLL) == Const::STATE_JUMP;
    }

    bool Enemy_State_Machine_Component::is_enemy_stun() {
        return this->get_current_state_at(Const::TRACK_EFFECTED) == Const::STATE_STUN;
    }

    void Enemy_State_Machine_Component::change_state_at(
        std::string track, std::string state, float state_duration, int source_call_tag, bool is_calling_end_last_state
    ) {
        State_Machine_Component::change_state_at(
            track, state, state_duration, source_call_tag, is_calling_end_last_state
        );
    }

    void Enemy_State_Machine_Component::handle_auto_change_state(
        Base_Node* target, std::string track, std::string state, void* global_data, int source_call_state
    ) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        Enemy_Node* enemy = cast_enemy_target(target);
        if (enemy == nullptr)
            return;
        const auto& behavior_config = enemy->get_behavior_config_from(global_data);
        Enemy_Data& enemy_data = enemy->get_enemy_data(global_data);
        Enemy_Behavior_Component* behavior = get_enemy_bihavior_component(target);

        if (this->is_enemy_dead()) {
            return;
        }

        if (track == Const::TRACK_CONTROLL) {
            if (this->is_enemy_attacked() || this->is_enemy_stun()) {
                return;
            }
            if (state == Const::STATE_SKILL_CHANNELLING) {
                if (this->get_source_call_state_at(Const::TRACK_CONTROLL, Const::STATE_SKILL_CHANNELLING) ==
                    Const::ENEMY_CHANNLING_BY_SKILL_THROWING) {
                    const Boss_Skill_Throw_Enemy_Config& skill_config =
                        data->get_config().get_boss_skill_throw_enemy_config();
                    this->change_state_at(
                        Const::TRACK_CONTROLL,
                        Const::STATE_FLIGHT,
                        skill_config.duration_throwing,
                        Const::ENEMY_FLIGHT_BY_SKILL_THROWING
                    );
                    return;
                }
            }
            if (state == Const::STATE_FLIGHT) {
                int a = 5;
            }
            if (behavior->can_attack()) {
                this->change_state_at(
                    Const::TRACK_CONTROLL, Const::STATE_ATTACK, behavior_config.get_enemy_attack_duration()
                );
            } else if (behavior->is_walking()) {
                this->change_state_at(
                    Const::TRACK_CONTROLL, Const::STATE_MOVE, behavior_config.get_enemy_walk_duration()
                );
            } else {
                this->change_state_at(
                    Const::TRACK_CONTROLL, Const::STATE_MOVE, behavior_config.get_enemy_walk_duration()
                );
            }
        }

        if (track == Const::TRACK_EFFECTED) {
            if (state == Const::STATE_STUN || state == Const::STATE_ATTACKED) {
                this->change_state_at(
                    Const::TRACK_EFFECTED, Const::STATE_UNEFFECTED, State_Machine_Component::INFITY_STATE
                );
                if (enemy_data.get_current_health() <= 0) {
                    this->change_state_at(
                        Const::TRACK_CONTROLL, Const::STATE_DEATH, State_Machine_Component::INFITY_STATE
                    );
                    return;
                }
                this->change_state_at(
                    Const::TRACK_CONTROLL, Const::STATE_MOVE, behavior_config.get_enemy_walk_duration()
                );
            }
        }
    }
}  // namespace Meow_Meow