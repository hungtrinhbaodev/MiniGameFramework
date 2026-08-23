#include <defined.h>
#include <meow_meow/animation/shoot_animation.h>
#include <meow_meow/global_data.h>
#include <meow_meow/layer/layer_battle.h>
#include <meow_meow/object/bullet_node.h>
#include <meow_meow/object/character_node.h>
#include <state_machine_component.h>
#include <utils.h>

namespace Meow_Meow {

    bool is_character_attacking(State_Machine_Component* state_machine_component) {
        return state_machine_component->get_current_state_at(Const::TRACK_CONTROLL) == Const::STATE_ATTACK &&
               !state_machine_component->is_finish_state_at(Const::TRACK_CONTROLL);
    }

    bool is_character_attacked(State_Machine_Component* State_Machine_Component) {
        return false;
    }

    bool can_process_move_input(State_Machine_Component* state_machine_component, Key_Input_Type type) {
        if (type != Key_Input_Type::PRESSED && type != Key_Input_Type::HOLDING)
            return false;
        if (is_character_attacking(state_machine_component)) {
            return false;
        }
        return true;
    }

    bool can_process_attack_input(State_Machine_Component* state_machine_component, Key_Input_Type type) {
        if (type != Key_Input_Type::PRESSED)
            return false;
        if (is_character_attacked(state_machine_component) && !is_character_attacking(state_machine_component)) {
            return false;
        }
        return true;
    }

    Character_Node::Character_Node() {
        this->init_character_animation();
        this->init_components();
        this->set_name("debug");
    }

    Character_Node::~Character_Node() {}

    void Character_Node::set_character_animation_id(int character_animation_id) {
        this->character_animation_id = character_animation_id;
    }

    void Character_Node::init_character_animation() {
        this->character_animtion = new Character_Animation(0, 2);
        this->add_child(this->character_animtion);
    }

    void Character_Node::init_components() {
        State_Machine_Component* state_machine = new State_Machine_Component();
        /**
         * Add track controll to state machine
         */
        state_machine->add_track(Const::TRACK_CONTROLL, nullptr);
        state_machine->add_track(Const::TRACK_EFFECTED, nullptr);
        state_machine->set_name(Defined::COMPONENT_STATE_MACHINE_NAME);
        this->add_component(state_machine);

        this->add_key_press_listener(Custom::Key::W);
        this->add_key_press_listener(Custom::Key::A);
        this->add_key_press_listener(Custom::Key::S);
        this->add_key_press_listener(Custom::Key::D);
        this->add_key_press_listener(Custom::Key::SPACE);
    }

    void Character_Node::attach(void* global_data) {
        State_Machine_Component* state_machine_component =
            Utils::get_component<State_Machine_Component>(this, Defined::COMPONENT_STATE_MACHINE_NAME);
        state_machine_component->change_state_at(
            Const::TRACK_CONTROLL, Const::STATE_IDLE, State_Machine_Component::INFITY_STATE
        );
        this->character_animtion->play_animation("IDLE");
    }

    Custom::Transformed_Rectangle Character_Node::get_bounding_box() {
        Custom::Size bounding_size = Const::CHARACTER_BOUNDING_BOX;
        Custom::Rectangle rect{bounding_size.width, bounding_size.height};
        return Custom::Transformed_Rectangle{rect.apply(this->get_transform(), {0.5, 0.5})};
    }

    void Character_Node::handle_boundary(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        Battle_Layer* layer = data->get_battle_layer();
        if (layer == nullptr)
            return;
        Custom::Size layer_size = layer->get_content_size();
        Custom::Size bounding_size = Const::CHARACTER_BOUNDING_BOX;
        glm::vec2 start_bounding_position = this->get_position() - glm::vec2{0.5, 0.5} * bounding_size.to_vec2();
        Custom::Rectangle_Area bounding_rect = {
            start_bounding_position.x, start_bounding_position.y, bounding_size.width, bounding_size.height
        };
        Custom::Rectangle_Area fix_with_layer_bounding_rect = bounding_rect;
        Custom::Rectangle_Area layer_rect = {0, 0, layer_size.width, layer_size.height};
        fix_with_layer_bounding_rect.fix_with(layer_rect);
        glm::vec2 delta_position = glm::vec2{
            fix_with_layer_bounding_rect.x - bounding_rect.x, fix_with_layer_bounding_rect.y - bounding_rect.y
        };
        this->set_position(this->get_position() + delta_position);
    }

    void Character_Node::update_moverment(float delta_time) {
        this->velocity -= this->accelarate * delta_time;
        this->velocity = {std::max(0.f, this->velocity.x), std::max(0.f, this->velocity.y)};
        this->set_position(this->get_position() + this->velocity * this->get_direction() * delta_time);
    }

    void Character_Node::update_character_direction() {
        this->character_animtion->set_flipped_x(this->horizontal_direction == Const::DIRECTION::LEFT);
    }

    glm::vec2 Character_Node::get_direction() {
        return {
            this->horizontal_direction == Const::DIRECTION::LEFT ? -1 : 1,
            this->vertical_direction == Const::DIRECTION::DOWN ? -1 : 1
        };
    }

    bool Character_Node::is_moving_at_direction(Const::DIRECTION direction) {
        return (this->horizontal_direction == direction && this->velocity.x > 0) ||
               (this->vertical_direction == direction && this->velocity.y > 0);
    }

    void Character_Node::change_to_move(Const::DIRECTION horizontal, Const::DIRECTION vertical, float duration_hold) {
        State_Machine_Component* state_machine_component =
            Utils::get_component<State_Machine_Component>(this, Defined::COMPONENT_STATE_MACHINE_NAME);

        std::string last_state = state_machine_component->get_last_state_processign_at(Const::TRACK_CONTROLL);
        if (last_state == Const::STATE_ATTACK) {
            this->character_animtion->play_animation("IDLE");
        }

        float bonus_velosity_rate = duration_hold / BONUS_VELOSITY_RATE;
        float bonus_velosity = std::min(MAX_BONUS_VELOSITY, BONUS_VELOSITY * bonus_velosity_rate);

        this->horizontal_direction = horizontal != Const::DIRECTION::NONE ? horizontal : this->horizontal_direction;
        this->vertical_direction = vertical != Const::DIRECTION::NONE ? vertical : this->vertical_direction;

        this->velocity.x = horizontal != Const::DIRECTION::NONE ? VELOCITY_MOVE_VALUE + bonus_velosity : 0;
        this->accelarate.x = horizontal != Const::DIRECTION::NONE ? ACCELARATE_MOVE_VALUE : 0;

        this->velocity.y = vertical != Const::DIRECTION::NONE ? VELOCITY_MOVE_VALUE + bonus_velosity : 0;
        this->accelarate.y = vertical != Const::DIRECTION::NONE ? ACCELARATE_MOVE_VALUE : 0;

        state_machine_component->change_state_at(
            Const::TRACK_CONTROLL, Const::STATE_MOVE, State_Machine_Component::INFITY_STATE
        );
    }

    void Character_Node::change_to_idle() {
        State_Machine_Component* state_machine_component =
            Utils::get_component<State_Machine_Component>(this, Defined::COMPONENT_STATE_MACHINE_NAME);

        std::string last_state = state_machine_component->get_last_state_processign_at(Const::TRACK_CONTROLL);
        if (last_state == Const::STATE_ATTACK) {
            this->character_animtion->play_animation("IDLE");
        }

        state_machine_component->change_state_at(
            Const::TRACK_CONTROLL, Const::STATE_IDLE, State_Machine_Component::INFITY_STATE
        );

        this->velocity = {0.f, 0.f};
        this->accelarate = {0.f, 0.f};
    }

    void Character_Node::change_to_attack(void* global_data) {
        State_Machine_Component* state_machine_component =
            Utils::get_component<State_Machine_Component>(this, Defined::COMPONENT_STATE_MACHINE_NAME);

        state_machine_component->change_state_at(Const::TRACK_CONTROLL, Const::STATE_ATTACK, DURATION_ATTACK);
        float animation_duration = this->character_animtion->get_amimation_duration("SHOOT");
        this->character_animtion->play_animation("SHOOT", DURATION_ATTACK / animation_duration, true);

        this->velocity = {0.f, 0.f};
        this->accelarate = {0.f, 0.f};

        /**
         * Add bullet to map when character fire!
         */
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        Battle_Layer* battle_layer = data->get_battle_layer();
        if (!battle_layer)
            return;

        Bullet_Node* bullet = new Bullet_Node(this->character_animation_id, this->horizontal_direction);
        float sign_x = horizontal_direction == Const::DIRECTION::LEFT ? -1 : 1;
        glm::vec2 fire_position = this->get_position() + glm::vec2{sign_x, 1} * DELTA_POSITION_BULLET;
        bullet->set_position(fire_position);
        battle_layer->add_child(bullet);

        Shoot_Animation* shoot_animation = new Shoot_Animation(fire_position, this->horizontal_direction);
        battle_layer->remove_child_by_tag(Const::SHOOT_ANIMATION_NODE_TAG);
        battle_layer->add_child(shoot_animation);
    }

    void Character_Node::handle_key_board(float delta_time, void* global_data) {
        Key_Input_Component* key_input =
            Utils::get_component<Key_Input_Component>(this, Defined::COMPONENT_KEY_INPUT_NAME);

        State_Machine_Component* state_machine_component =
            Utils::get_component<State_Machine_Component>(this, Defined::COMPONENT_STATE_MACHINE_NAME);

        if (key_input == nullptr || state_machine_component == nullptr)
            return;

        std::map<Custom::Key, Key_Press_Detail> keys_pressed = key_input->get_key_inputs();
        for (auto& [key, detail] : keys_pressed) {
            switch (key) {
                case Custom::Key::W: {
                    if (can_process_move_input(state_machine_component, detail.type)) {
                        this->change_to_move(Const::DIRECTION::NONE, Const::DIRECTION::UP, detail.duration_pressed);
                        break;
                    }
                    if (!is_character_attacking(state_machine_component) &&
                        is_moving_at_direction(Const::DIRECTION::UP)) {
                        this->change_to_idle();
                    }
                    break;
                }
                case Custom::Key::S: {
                    if (can_process_move_input(state_machine_component, detail.type)) {
                        this->change_to_move(Const::DIRECTION::NONE, Const::DIRECTION::DOWN, detail.duration_pressed);
                        break;
                    }
                    if (!is_character_attacking(state_machine_component) &&
                        is_moving_at_direction(Const::DIRECTION::DOWN)) {
                        this->change_to_idle();
                    }
                    break;
                }
                case Custom::Key::A: {
                    if (can_process_move_input(state_machine_component, detail.type)) {
                        this->change_to_move(Const::DIRECTION::LEFT, Const::DIRECTION::NONE, detail.duration_pressed);
                        break;
                    }
                    if (!is_character_attacking(state_machine_component) &&
                        is_moving_at_direction(Const::DIRECTION::LEFT)) {
                        this->change_to_idle();
                    }
                    break;
                }
                case Custom::Key::D: {
                    if (can_process_move_input(state_machine_component, detail.type)) {
                        this->change_to_move(Const::DIRECTION::RIGHT, Const::DIRECTION::NONE, detail.duration_pressed);
                        break;
                    }
                    if (!is_character_attacking(state_machine_component) &&
                        is_moving_at_direction(Const::DIRECTION::RIGHT)) {
                        this->change_to_idle();
                    }
                    break;
                }
                case Custom::Key::SPACE: {
                    if (can_process_attack_input(state_machine_component, detail.type)) {
                        this->change_to_attack(global_data);
                    }
                    break;
                }
                default: {
                    break;
                }
            }
        }
    }

    void Character_Node::handle_state_machine(float delta_time, void* global_data) {
        /**
         * @Note: Handle auto change state of state machine here!
         */
        State_Machine_Component* state_machine_component =
            Utils::get_component<State_Machine_Component>(this, Defined::COMPONENT_STATE_MACHINE_NAME);
        if (state_machine_component->is_finish_state_at(Const::TRACK_CONTROLL)) {
            std::string current_state = state_machine_component->get_current_state_at(Const::TRACK_CONTROLL);
            if (current_state == Const::STATE_ATTACK) {
                this->change_to_idle();
            }
        }
    }

    void Character_Node::fix_update(float delta_time, void* global_data) {
        this->handle_key_board(delta_time, global_data);
        this->handle_state_machine(delta_time, global_data);
        this->update_moverment(delta_time);
        this->update_character_direction();
        Game_Object::fix_update(delta_time, global_data);
    }
}  // namespace Meow_Meow