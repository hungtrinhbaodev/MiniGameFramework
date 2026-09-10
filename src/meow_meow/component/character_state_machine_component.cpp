#include <meow_meow/component/character_state_machine_component.h>
#include <meow_meow/const.h>
#include <meow_meow/data/global_data.h>

namespace Meow_Meow {
    Character_State_Machine_Component::Character_State_Machine_Component() {}

    Character_State_Machine_Component::~Character_State_Machine_Component() {}

    bool Character_State_Machine_Component::is_character_attacking() {
        return this->get_current_state_at(Const::TRACK_CONTROLL) == Const::STATE_ATTACK;
    }

    bool Character_State_Machine_Component::is_character_attacked() {
        return this->get_current_state_at(Const::TRACK_EFFECTED) == Const::STATE_ATTACKED;
    }

    bool Character_State_Machine_Component::is_character_channelling() {
        return this->get_current_state_at(Const::TRACK_CONTROLL) == Const::STATE_SKILL_CHANNELLING;
    }

    bool Character_State_Machine_Component::is_character_dead() {
        return this->get_current_state_at(Const::TRACK_CONTROLL) == Const::STATE_DEATH;
    }

    bool Character_State_Machine_Component::is_character_flight() {
        return this->get_current_state_at(Const::TRACK_CONTROLL) == Const::STATE_FLIGHT;
    }

    bool Character_State_Machine_Component::is_character_stunned() {
        return this->is_character_flight() || this->get_current_state_at(Const::TRACK_EFFECTED) == Const::STATE_STUN;
    }

    bool Character_State_Machine_Component::is_character_moving() {
        return this->get_current_state_at(Const::TRACK_CONTROLL) == Const::STATE_MOVE;
    }

    bool Character_State_Machine_Component::is_character_invincible() {
        return this->get_current_state_at(Const::TRACK_EFFECTED) == Const::STATE_INVINCIBLE;
    }

    bool Character_State_Machine_Component::is_character_dashing() {
        return this->get_current_state_at(Const::TRACK_CONTROLL) == Const::STATE_DASHING;
    }

    bool Character_State_Machine_Component::can_character_attack(void* global_data) {
        if (this->is_character_attacking() || this->is_character_attacked() || this->is_character_dashing() ||
            this->is_character_channelling() || this->is_character_stunned()) {
            return false;
        }
        return true;
    }

    bool Character_State_Machine_Component::can_process_move_input(void* global_data) {
        if (this->is_character_attacking() || this->is_character_attacked() || this->is_character_dashing() ||
            this->is_character_channelling() || this->is_character_stunned()) {
            return false;
        }
        return true;
    }

    bool Character_State_Machine_Component::can_process_release_move(void* global_data) {
        if (this->is_character_attacking() || this->is_character_attacked() || this->is_character_dashing() ||
            this->is_character_channelling() || this->is_character_stunned()) {
            return false;
        }
        if (!this->is_character_moving()) {
            return false;
        }
        return true;
    }

    void Character_State_Machine_Component::handle_auto_change_state(
        Base_Node* target, std::string track, std::string state, void* global_data, int source_call_state
    ) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        Player_Data& player_data = data->get_player_data();
        const Character_Behavior_Config& behavior_config = data->get_config().get_character_behavior_config();

        if (track == Const::TRACK_CONTROLL) {
            if (state == Const::STATE_ATTACK) {
                this->change_state_at(Const::TRACK_CONTROLL, Const::STATE_IDLE, State_Machine_Component::INFITY_STATE);
            } else if (state == Const::STATE_DASHING) {
                this->change_state_at(Const::TRACK_CONTROLL, Const::STATE_IDLE, State_Machine_Component::INFITY_STATE);
            } else if (state == Const::STATE_FLIGHT) {
                if (player_data.get_current_health() <= 0) {
                    this->change_state_at(
                        Const::TRACK_CONTROLL, Const::STATE_DEATH, behavior_config.get_dead_duration()
                    );
                } else {
                    switch (source_call_state) {
                        case Const::CHARACTER_FLIGHT_FROM_BOSS_SKILL_FLASH: {
                            const Boss_Skill_Flash_Config& skill_config =
                                data->get_config().get_boss_skill_flash_config();
                            this->change_state_at(
                                Const::TRACK_EFFECTED,
                                Const::STATE_STUN,
                                skill_config.stun_duration,
                                Const::CHARACTER_STUN_FROM_BOSS_SKILL_FLASH
                            );
                            break;
                        }
                    }
                    this->change_state_at(
                        Const::TRACK_CONTROLL, Const::STATE_IDLE, State_Machine_Component::INFITY_STATE
                    );
                }
            } else if (state == Const::STATE_SKILL_CHANNELLING) {
                this->change_state_at(Const::TRACK_CONTROLL, Const::STATE_IDLE, State_Machine_Component::INFITY_STATE);
            } else if (state == Const::STATE_DEATH) {
                return;
            }
        }

        if (track == Const::TRACK_EFFECTED) {
            if (state == Const::STATE_ATTACKED) {
                if (player_data.get_current_health() <= 0) {
                    this->change_state_at(
                        Const::TRACK_CONTROLL, Const::STATE_DEATH, behavior_config.get_dead_duration()
                    );
                    this->change_state_at(
                        Const::TRACK_EFFECTED, Const::STATE_UNEFFECTED, behavior_config.get_invincible_duration()
                    );
                } else {
                    this->change_state_at(
                        Const::TRACK_EFFECTED, Const::STATE_INVINCIBLE, behavior_config.get_invincible_duration()
                    );
                }
            } else if (state == Const::STATE_INVINCIBLE) {
                this->change_state_at(
                    Const::TRACK_EFFECTED, Const::STATE_UNEFFECTED, State_Machine_Component::INFITY_STATE
                );
            } else if (state == Const::STATE_STUN) {
                this->change_state_at(
                    Const::TRACK_EFFECTED, Const::STATE_UNEFFECTED, State_Machine_Component::INFITY_STATE
                );
            }
        }
    }
}  // namespace Meow_Meow