#include <defined.h>
#include <meow_meow/const.h>
#include <meow_meow/object/character_node.h>
#include <state_machine_component.h>
#include <utils.h>

namespace Meow_Meow {

    bool is_character_attacking(State_Machine_Component* state_machine_component) {
        return state_machine_component->get_current_state_at("CONTROLL") == "ATTACK" &&
               !state_machine_component->is_finish_state_at("CONTROLL");
    }

    bool is_character_attacked(State_Machine_Component* State_Machine_Component) {
        return false;
    }

    Character_Node::Character_Node() {
        this->init_character_animation();
        this->init_components();
    }

    Character_Node::~Character_Node() {}

    void Character_Node::init_character_animation() {
        this->character_animtion = new Character_Animation(0, 2);
        this->add_child(this->character_animtion);
    }

    void Character_Node::init_components() {
        State_Machine_Component* state_machine = new State_Machine_Component();
        /**
         * Add track controll to state machine
         */
        state_machine->add_track("CONTROLL", nullptr);
        state_machine->add_track("EFFECTED", nullptr);
        state_machine->set_name(Const::CHARACTER_STATE_MACHINE_NAME);
        this->add_component(state_machine);

        this->add_key_press_listener(Custom::Key::W);
        this->add_key_press_listener(Custom::Key::A);
        this->add_key_press_listener(Custom::Key::S);
        this->add_key_press_listener(Custom::Key::D);
        this->add_key_press_listener(Custom::Key::SPACE);
    }

    void Character_Node::attach(void* global_data) {
        State_Machine_Component* state_machine_component =
            Utils::get_component<State_Machine_Component>(this, Const::CHARACTER_STATE_MACHINE_NAME);
        state_machine_component->change_state_at("CONTROLL", "IDLE", 0);
        this->character_animtion->play_animation("IDLE");
    }

    void Character_Node::update_moverment(float delta_time) {
        this->velocity -= this->accelarate * delta_time;
        this->velocity = {std::max(0.f, this->velocity.x), std::max(0.f, this->velocity.y)};
        this->set_position(this->get_position() + this->velocity * this->direction * delta_time);
    }

    bool Character_Node::finish_moving() {
        return this->velocity.x <= 0 && this->velocity.y <= 0;
    }

    void Character_Node::handle_key_board(float delta_time, void* global_data) {
        Key_Input_Component* key_input =
            Utils::get_component<Key_Input_Component>(this, Defined::COMPONENT_KEY_INPUT_NAME);
        State_Machine_Component* state_machine_component =
            Utils::get_component<State_Machine_Component>(this, Const::CHARACTER_STATE_MACHINE_NAME);
        if (key_input == nullptr || state_machine_component == nullptr)
            return;
        std::map<Custom::Key, Key_Input_Type> keys_pressed = key_input->get_key_inputs();
        for (auto& [key, type] : keys_pressed) {
            switch (key) {
                case Custom::Key::W: {
                    if (type != Key_Input_Type::PRESSED && type != Key_Input_Type::HOLDING)
                        break;
                    if (is_character_attacking(state_machine_component)) {
                        break;
                    }
                    this->direction.y = 1;
                    this->velocity = {0, VELOCITY_MOVE_VALUE};
                    this->accelarate = {0, ACCELARATE_MOVE_VALUE};
                    state_machine_component->change_state_at("CONTROLL", "MOVE", 0);
                    break;
                }
                case Custom::Key::S: {
                    if (type != Key_Input_Type::PRESSED && type != Key_Input_Type::HOLDING)
                        break;
                    if (is_character_attacking(state_machine_component)) {
                        break;
                    }
                    this->direction.y = -1;
                    this->velocity = {0, VELOCITY_MOVE_VALUE};
                    this->accelarate = {0, ACCELARATE_MOVE_VALUE};
                    state_machine_component->change_state_at("CONTROLL", "MOVE", 0);
                    break;
                }
                case Custom::Key::A: {
                    if (type != Key_Input_Type::PRESSED && type != Key_Input_Type::HOLDING)
                        break;
                    if (is_character_attacking(state_machine_component)) {
                        break;
                    }
                    this->direction.x = -1;
                    this->velocity = {VELOCITY_MOVE_VALUE, 0};
                    this->accelarate = {VELOCITY_MOVE_VALUE, 0};
                    state_machine_component->change_state_at("CONTROLL", "MOVE", 0);
                    break;
                }
                case Custom::Key::D: {
                    if (type != Key_Input_Type::PRESSED && type != Key_Input_Type::HOLDING)
                        break;
                    if (is_character_attacking(state_machine_component)) {
                        break;
                    }
                    this->direction.x = 1;
                    this->velocity = {VELOCITY_MOVE_VALUE, 0};
                    this->accelarate = {VELOCITY_MOVE_VALUE, 0};
                    state_machine_component->change_state_at("CONTROLL", "MOVE", 0);
                    break;
                }
                case Custom::Key::SPACE: {
                    if (type != Key_Input_Type::PRESSED)
                        break;
                    if (is_character_attacked(state_machine_component) &&
                        !is_character_attacked(state_machine_component)) {
                        break;
                    }
                    /**
                     * TODO: add attack speed rate here latter!
                     */
                    this->velocity = {0, 0};
                    this->accelarate = {0, 0};
                    state_machine_component->change_state_at("CONTROLL", "ATTACK", 0.35);
                    this->character_animtion->play_animation("SHOOT");
                    break;
                }
                default: {
                    break;
                }
            }
        }
    }

    void Character_Node::handle_state_machine(float delta_time, void* global_data) {
        State_Machine_Component* state_machine_component =
            Utils::get_component<State_Machine_Component>(this, Const::CHARACTER_STATE_MACHINE_NAME);
        if (state_machine_component->is_finish_state_at("CONTROLL")) {
            std::string current_state = state_machine_component->get_current_state_at("CONTROLL");
            std::string last_state = state_machine_component->get_last_state_processign_at("CONTROLL");
            if (current_state == "ATTACK") {
                state_machine_component->change_state_at("CONTROLL", "IDLE", 0);
                this->character_animtion->play_animation("IDLE");
            } else if (current_state == "MOVE" && this->finish_moving()) {
                state_machine_component->change_state_at("CONTROLL", "IDLE", 0);
            }
        }
    }

    void Character_Node::fix_update(float delta_time, void* global_data) {
        this->handle_key_board(delta_time, global_data);
        this->handle_state_machine(delta_time, global_data);
        this->update_moverment(delta_time);
    }
}  // namespace Meow_Meow