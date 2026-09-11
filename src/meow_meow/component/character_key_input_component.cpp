#include <defined.h>
#include <meow_meow/const.h>
#include <meow_meow/data/global_data.h>
#include <meow_meow/object/character_node.h>
#include <meow_meow/utils.h>
#include <utils.h>

#include <array>

namespace Meow_Meow {

    Character_Key_Input_Component::Character_Key_Input_Component() {}

    Character_Key_Input_Component::~Character_Key_Input_Component() {}

    Custom::Key Character_Key_Input_Component::get_key_need_handled() {
        return this->key_need_handled;
    }

    Key_Input_Type Character_Key_Input_Component::get_key_type_pressed() {
        return this->key_type_pressed;
    }

    float Character_Key_Input_Component::get_duration_hold() {
        return this->duration_hold;
    }

    bool Character_Key_Input_Component::is_pressed_key(Custom::Key key) {
        std::map<Custom::Key, Key_Press_Detail> handled_keys = this->get_key_inputs();
        if (handled_keys.find(key) == handled_keys.end()) {
            return false;
        }
        return handled_keys[key].type == Key_Input_Type::PRESSED;
    }

    bool Character_Key_Input_Component::is_holding_key(Custom::Key key) {
        std::map<Custom::Key, Key_Press_Detail> handled_keys = this->get_key_inputs();
        if (handled_keys.find(key) == handled_keys.end()) {
            return false;
        }
        return handled_keys[key].type == Key_Input_Type::HOLDING;
    }

    bool Character_Key_Input_Component::is_released_key(Custom::Key key) {
        std::map<Custom::Key, Key_Press_Detail> handled_keys = this->get_key_inputs();
        if (handled_keys.find(key) == handled_keys.end()) {
            return false;
        }
        return handled_keys[key].type == Key_Input_Type::RELEASE;
    }

    bool Character_Key_Input_Component::is_cancel_key(Custom::Key key) {
        std::map<Custom::Key, Key_Press_Detail> handled_keys = this->get_key_inputs();
        if (handled_keys.find(key) == handled_keys.end()) {
            return false;
        }
        return handled_keys[key].type == Key_Input_Type::CANCEL;
    }

    void Character_Key_Input_Component::set_pressed_information(
        Custom::Key key_need_handled, Key_Input_Type key_type_pressed, float duration_hold
    ) {
        this->key_need_handled = key_need_handled;
        this->key_type_pressed = key_type_pressed;
        this->duration_hold = duration_hold;
    }

    void Character_Key_Input_Component::handle_task(Base_Node* target, float delta_time, void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const Character_Behavior_Config& behavior_config = data->get_config().get_character_behavior_config();

        std::map<Custom::Key, Key_Press_Detail> handled_keys = this->get_key_inputs();
        Character_Node* character = cast_charater_target(target);
        Character_State_Machine_Component* state_machine = get_character_state_machine(target);
        if (state_machine == nullptr)
            return;

        /**
         * Handle player using skill thunder
         */
        Character_Skill_Thunder_Component* thunder_skill = get_skill_thunder_component(target);
        if (thunder_skill != nullptr) {
            Custom::Key KEY_THUNDER = Custom::Key::V;
            if (this->is_pressed_key(KEY_THUNDER) && thunder_skill->can_activate_skill(state_machine, global_data)) {
                const Character_Skill_Thunder_Config& skill_thunder_config =
                    data->get_config().get_character_skill_thunder_config();
                state_machine->change_state_at(
                    Const::TRACK_CONTROLL,
                    Const::STATE_SKILL_CHANNELLING,
                    skill_thunder_config.channelling_duration,
                    Const::CHARACTER_CHANNLING_FROM_SKILL_THUNDER
                );
            }
        }

        /**
         * Handle player using skill dash
         */
        Character_Skill_Dash_Component* dash_skill = get_skill_dash_component(target);
        if (dash_skill != nullptr) {
            Custom::Key KEY_DASH = Custom::Key::C;
            if (this->is_pressed_key(KEY_DASH) && dash_skill->can_activate_skill(state_machine, global_data)) {
                const Character_Skill_Dash_Config& dash_skill_config =
                    data->get_config().get_character_skill_dash_config();
                state_machine->change_state_at(
                    Const::TRACK_CONTROLL, Const::STATE_DASHING, dash_skill_config.dash_duration
                );
            }
        }

        /**
         * Handle player press attack
         */
        Custom::Key KEY_ATTACK = Custom::Key::SPACE;
        if (this->is_pressed_key(KEY_ATTACK) && state_machine->can_character_attack(global_data)) {
            state_machine->change_state_at(
                Const::TRACK_CONTROLL, Const::STATE_ATTACK, behavior_config.get_attack_duration()
            );
        }

        /**
         * Handle player move
         */
        std::array<Custom::Key, 4> KEYS_MOVE = {Custom::Key::W, Custom::Key::A, Custom::Key::S, Custom::Key::D};
        for (Custom::Key KEY_MOVE : KEYS_MOVE) {
            bool press_or_hold_key = this->is_pressed_key(KEY_MOVE) || this->is_holding_key(KEY_MOVE);
            bool release_or_cancel_key = this->is_released_key(KEY_MOVE) || this->is_cancel_key(KEY_MOVE);
            switch (KEY_MOVE) {
                case Custom::Key::W: {
                    if (press_or_hold_key && state_machine->can_process_move_input(global_data, Const::DIRECTION::UP)) {
                        this->set_pressed_information(
                            KEY_MOVE, handled_keys[KEY_MOVE].type, handled_keys[KEY_MOVE].duration_pressed
                        );
                        if (state_machine->is_character_dashing()) {
                            state_machine->change_state_at(
                                Const::TRACK_CONTROLL,
                                Const::STATE_DASHING,
                                state_machine->get_duration_remain_state_at(Const::TRACK_CONTROLL),
                                Const::MIX_DASH_WITH_MOVE,
                                false
                            );
                        } else {
                            state_machine->change_state_at(
                                Const::TRACK_CONTROLL, Const::STATE_MOVE, State_Machine_Component::INFITY_STATE
                            );
                        }
                    }
                    if (release_or_cancel_key && state_machine->can_process_release_move(global_data) &&
                        character->is_moving_at_direction(Const::DIRECTION::UP)) {
                        state_machine->change_state_at(
                            Const::TRACK_CONTROLL, Const::STATE_IDLE, State_Machine_Component::INFITY_STATE
                        );
                    }
                    break;
                }
                case Custom::Key::A: {
                    if (press_or_hold_key &&
                        state_machine->can_process_move_input(global_data, Const::DIRECTION::LEFT)) {
                        this->set_pressed_information(
                            KEY_MOVE, handled_keys[KEY_MOVE].type, handled_keys[KEY_MOVE].duration_pressed
                        );
                        state_machine->change_state_at(
                            Const::TRACK_CONTROLL, Const::STATE_MOVE, State_Machine_Component::INFITY_STATE
                        );
                    }
                    if (release_or_cancel_key && state_machine->can_process_release_move(global_data) &&
                        character->is_moving_at_direction(Const::DIRECTION::LEFT)) {
                        state_machine->change_state_at(
                            Const::TRACK_CONTROLL, Const::STATE_IDLE, State_Machine_Component::INFITY_STATE
                        );
                    }
                    break;
                }
                case Custom::Key::S: {
                    if (press_or_hold_key &&
                        state_machine->can_process_move_input(global_data, Const::DIRECTION::DOWN)) {
                        this->set_pressed_information(
                            KEY_MOVE, handled_keys[KEY_MOVE].type, handled_keys[KEY_MOVE].duration_pressed
                        );
                        if (state_machine->is_character_dashing()) {
                            state_machine->change_state_at(
                                Const::TRACK_CONTROLL,
                                Const::STATE_DASHING,
                                state_machine->get_duration_remain_state_at(Const::TRACK_CONTROLL),
                                Const::MIX_DASH_WITH_MOVE,
                                false
                            );
                        } else {
                            state_machine->change_state_at(
                                Const::TRACK_CONTROLL, Const::STATE_MOVE, State_Machine_Component::INFITY_STATE
                            );
                        }
                    }
                    if (release_or_cancel_key && state_machine->can_process_release_move(global_data) &&
                        character->is_moving_at_direction(Const::DIRECTION::DOWN)) {
                        state_machine->change_state_at(
                            Const::TRACK_CONTROLL, Const::STATE_IDLE, State_Machine_Component::INFITY_STATE
                        );
                    }
                    break;
                }
                case Custom::Key::D: {
                    if (press_or_hold_key &&
                        state_machine->can_process_move_input(global_data, Const::DIRECTION::RIGHT)) {
                        this->set_pressed_information(
                            KEY_MOVE, handled_keys[KEY_MOVE].type, handled_keys[KEY_MOVE].duration_pressed
                        );
                        state_machine->change_state_at(
                            Const::TRACK_CONTROLL, Const::STATE_MOVE, State_Machine_Component::INFITY_STATE
                        );
                    }
                    if (release_or_cancel_key && state_machine->can_process_release_move(global_data) &&
                        character->is_moving_at_direction(Const::DIRECTION::RIGHT)) {
                        state_machine->change_state_at(
                            Const::TRACK_CONTROLL, Const::STATE_IDLE, State_Machine_Component::INFITY_STATE
                        );
                    }
                    break;
                }
                default: {
                    break;
                }
            }
        }
    }
}  // namespace Meow_Meow