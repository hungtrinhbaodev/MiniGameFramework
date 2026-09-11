#include <actions.h>
#include <collision_component.h>
#include <defined.h>
#include <label_node.h>
#include <math_custom.h>
#include <meow_meow/animation/character_channelling_animation.h>
#include <meow_meow/animation/character_fire_ball_anmimation.h>
#include <meow_meow/animation/level_up_flame_animation.h>
#include <meow_meow/animation/shoot_animation.h>
#include <meow_meow/component/character_collision_component.h>
#include <meow_meow/component/character_key_input_component.h>
#include <meow_meow/component/character_skill_dash_component.h>
#include <meow_meow/component/character_skill_thunder_component.h>
#include <meow_meow/component/character_state_machine_component.h>
#include <meow_meow/config/character_skill_dash_config.h>
#include <meow_meow/data/boss_collision_data.h>
#include <meow_meow/data/enemy_collision_data.h>
#include <meow_meow/data/global_data.h>
#include <meow_meow/layer/layer_battle.h>
#include <meow_meow/object/bullet_node.h>
#include <meow_meow/object/character_node.h>
#include <utils.h>

namespace Meow_Meow {
    Character_Node::Character_Node() {
        this->init_container();
        this->init_character_animation();
        this->init_attacked_image();
        this->init_components();
        this->set_name("Character_Node");
    }

    Character_Node::~Character_Node() {}

    void Character_Node::init_character_animation() {
        this->character_animation = new Character_Animation(0, 2);
        this->container->add_child(this->character_animation);
    }

    void Character_Node::init_attacked_image() {
        this->attacked_image = new Image_UI_Node();
        this->attacked_image->set_force_renderer_color(Custom::Color{255, 0, 0});
        this->attacked_image->set_enable_force_renderer_color(true);
        this->attacked_image->set_visible(false);
        this->attacked_image->set_opacity(ORIGIN_ATTACKED_IMAGE_OPACITY);
        this->container->add_child(this->attacked_image);
        Utils::save_transform_origin(this->attacked_image);
    }

    void Character_Node::init_container() {
        this->container = new Node();
        this->container->set_cascade_opacity(true);
        this->add_child(this->container);
        Utils::save_transform_origin(this->container);
    }

    void Character_Node::init_components() {
        Character_State_Machine_Component* state_machine = new Character_State_Machine_Component();
        /**
         * Add track controll to state machine
         */
        state_machine->add_track(Const::TRACK_CONTROLL, nullptr);
        state_machine->add_track(Const::TRACK_EFFECTED, nullptr);
        state_machine->set_name(Defined::COMPONENT_STATE_MACHINE_NAME);
        this->set_up_state_machine_states(state_machine);
        this->add_component(state_machine);

        Collision_Component* collision = new Character_Collision_Component();
        collision->set_name(Defined::COMPONENT_COLLISION_NAME);
        collision->set_track_layer(Const::BATTLE_LAYER_COLLISION);
        this->add_component(collision);

        Character_Skill_Dash_Component* skill_dash = new Character_Skill_Dash_Component();
        skill_dash->set_name(Const::CHARACTER_SKILL_DASH_COMPONENT_NAME);
        this->add_component(skill_dash);

        Character_Skill_Thunder_Component* skill_thunder = new Character_Skill_Thunder_Component();
        skill_thunder->set_name(Const::CHARACTER_SKILL_THUNDER_COMPONENT_NAME);
        this->add_component(skill_thunder);

        Character_Key_Input_Component* key_input = new Character_Key_Input_Component();
        key_input->set_name(Defined::COMPONENT_KEY_INPUT_NAME);
        this->add_component(key_input);

        this->add_key_press_listener(Custom::Key::W);
        this->add_key_press_listener(Custom::Key::A);
        this->add_key_press_listener(Custom::Key::S);
        this->add_key_press_listener(Custom::Key::D);
        this->add_key_press_listener(Custom::Key::C /** Skill dash */);
        this->add_key_press_listener(Custom::Key::V /** Skill thunder */);
        this->add_key_press_listener(Custom::Key::SPACE);
    }

    void Character_Node::set_up_state_machine_states(State_Machine_Component* state_machine) {
        /**
         * Add state idle
         */
        state_machine->add_state_at(
            Const::TRACK_CONTROLL,
            Const::STATE_IDLE,
            [this](State_Machine_Component::State_Machine_Callback_Data callback) { this->start_idle(); },
            nullptr
        );
        /**
         * Add state move
         */
        state_machine->add_state_at(
            Const::TRACK_CONTROLL,
            Const::STATE_MOVE,
            [this](State_Machine_Component::State_Machine_Callback_Data callback) {
                this->start_move(callback.global_data);
            },
            [this](State_Machine_Component::State_Machine_Callback_Data callback) {
                this->end_move(callback.global_data, callback.next_state);
            }
        );
        /**
         * Add state attack
         */
        state_machine->add_state_at(
            Const::TRACK_CONTROLL,
            Const::STATE_ATTACK,
            [this](State_Machine_Component::State_Machine_Callback_Data callback) {
                this->start_attack(callback.global_data);
            },
            [this](State_Machine_Component::State_Machine_Callback_Data callback) {
                this->end_attack(callback.global_data);
            }
        );
        /**
         * Add state dead
         */
        state_machine->add_state_at(
            Const::TRACK_CONTROLL,
            Const::STATE_DEATH,
            [this](State_Machine_Component::State_Machine_Callback_Data callback) {
                this->start_dead(callback.global_data);
            },
            [this](State_Machine_Component::State_Machine_Callback_Data callback) {
                this->end_dead(callback.global_data);
            }
        );
        /**
         * Add state dash
         */
        state_machine->add_state_at(
            Const::TRACK_CONTROLL,
            Const::STATE_DASHING,
            [this](State_Machine_Component::State_Machine_Callback_Data callback) {
                this->start_dash(callback.global_data, callback.source_call_state);
            },
            [this](State_Machine_Component::State_Machine_Callback_Data callback) {
                this->end_dash(callback.global_data, callback.source_call_state);
            }
        );
        /**
         * Add state channeling
         */
        state_machine->add_state_at(
            Const::TRACK_CONTROLL,
            Const::STATE_SKILL_CHANNELLING,
            [this](State_Machine_Component::State_Machine_Callback_Data callback) {
                this->start_channelling(callback.global_data, callback.source_call_state);
            },
            [this](State_Machine_Component::State_Machine_Callback_Data callback) {
                this->end_channelling(callback.global_data, callback.source_call_state);
            }
        );
        /**
         * Add state channeling
         */
        state_machine->add_state_at(
            Const::TRACK_CONTROLL,
            Const::STATE_FLIGHT,
            [this](State_Machine_Component::State_Machine_Callback_Data callback) {
                this->start_flight(callback.global_data, callback.source_call_state);
            },
            [this](State_Machine_Component::State_Machine_Callback_Data callback) {
                this->end_flight(callback.global_data, callback.source_call_state);
            }
        );

        /**
         * Add state attacked
         */
        state_machine->add_state_at(
            Const::TRACK_EFFECTED,
            Const::STATE_ATTACKED,
            [this](State_Machine_Component::State_Machine_Callback_Data callback) {
                this->start_hitted(callback.global_data, callback.source_call_state);
            },
            [this](State_Machine_Component::State_Machine_Callback_Data callback) {
                this->end_hitted(callback.global_data, callback.source_call_state);
            }
        );
        /**
         * Add state stun
         */
        state_machine->add_state_at(
            Const::TRACK_EFFECTED,
            Const::STATE_STUN,
            [this](State_Machine_Component::State_Machine_Callback_Data callback) {
                this->start_stun(callback.global_data, callback.source_call_state);
            },
            [this](State_Machine_Component::State_Machine_Callback_Data callback) {
                this->end_stun(callback.global_data, callback.source_call_state);
            }
        );
        /**
         * Add state invincible
         */
        state_machine->add_state_at(
            Const::TRACK_EFFECTED,
            Const::STATE_INVINCIBLE,
            [this](State_Machine_Component::State_Machine_Callback_Data callback) {
                this->start_invincible(callback.global_data);
            },
            [this](State_Machine_Component::State_Machine_Callback_Data callback) {
                this->end_invincible(callback.global_data);
            }
        );
    }

    void Character_Node::attach(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const Character_Behavior_Config& behavior_config = data->get_config().get_character_behavior_config();
        Player_Data& player_data = data->get_player_data();

        State_Machine_Component* state_machine_component =
            Utils::get_component<State_Machine_Component>(this, Defined::COMPONENT_STATE_MACHINE_NAME);

        Collision_Component* collision =
            Utils::get_component<Collision_Component>(this, Defined::COMPONENT_COLLISION_NAME);
        collision->set_box_size(behavior_config.get_bounding_box());

        state_machine_component->change_state_at(
            Const::TRACK_EFFECTED, Const::STATE_UNEFFECTED, State_Machine_Component::INFITY_STATE
        );

        this->player_level = player_data.get_character_level();
        this->character_animation->set_character_id(player_data.get_character_animation_id());
        this->character_animation->set_character_level(this->player_level);

        state_machine_component->change_state_at(
            Const::TRACK_CONTROLL, Const::STATE_IDLE, State_Machine_Component::INFITY_STATE
        );
    }

    Custom::Transformed_Rectangle Character_Node::get_bounding_box(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const Character_Behavior_Config& behavior_config = data->get_config().get_character_behavior_config();

        Custom::Size bounding_size = behavior_config.get_bounding_box();
        Custom::Rectangle rect{bounding_size.width, bounding_size.height};
        return Custom::Transformed_Rectangle{rect.apply(this->get_transform(), {0.5, 0.5})};
    }

    void Character_Node::handle_boundary(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const Character_Behavior_Config& behavior_config = data->get_config().get_character_behavior_config();

        Battle_Layer* layer = data->get_battle_layer();
        if (layer == nullptr)
            return;
        Custom::Size layer_size = layer->get_content_size();
        Custom::Size bounding_size = behavior_config.get_bounding_box();
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
        this->character_animation->set_flipped_x(this->horizontal_direction == Const::DIRECTION::LEFT);
    }

    void Character_Node::sync_attacked_image() {
        if (!this->attacked_image->is_visible())
            return;
        this->attacked_image->set_image(this->character_animation->get_image());
        this->attacked_image->set_anchor(this->character_animation->get_anchor().to_vec2());
        this->attacked_image->set_flipped_x(this->character_animation->is_flipped_x());
    }

    void Character_Node::sync_player_data(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        Player_Data& player_data = data->get_player_data();
        if (this->player_level != player_data.get_character_level()) {
            this->player_level = player_data.get_character_level();
            this->character_animation->set_character_level(this->player_level);
        }
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

    void Character_Node::start_idle() {
        this->velocity = {0.f, 0.f};
        this->accelarate = {0.f, 0.f};
        this->character_animation->play_animation("IDLE");
    }

    void Character_Node::start_move(void* global_data) {
        Character_Key_Input_Component* input_component =
            Utils::get_component<Character_Key_Input_Component>(this, Defined::COMPONENT_KEY_INPUT_NAME);

        if (input_component == nullptr) {
            return;
        }

        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const Character_Behavior_Config& behavior_config = data->get_config().get_character_behavior_config();

        this->character_animation->play_animation("IDLE");
        float duration_hold = input_component->get_duration_hold();
        Const::DIRECTION horizontal = Const::DIRECTION::NONE;
        Const::DIRECTION vertical = Const::DIRECTION::NONE;

        switch (input_component->get_key_need_handled()) {
            case Custom::Key::W: {
                vertical = Const::DIRECTION::UP;
                break;
            }
            case Custom::Key::A: {
                horizontal = Const::DIRECTION::LEFT;
                break;
            }
            case Custom::Key::S: {
                vertical = Const::DIRECTION::DOWN;
                break;
            }
            case Custom::Key::D: {
                horizontal = Const::DIRECTION::RIGHT;
                break;
            }
            default: {
                break;
            }
        }

        float bonus_velosity_rate = duration_hold / behavior_config.get_bonus_velosity_rate();
        float bonus_velosity = std::min(
            behavior_config.get_max_bonus_velosity(), behavior_config.get_bonus_velosity() * bonus_velosity_rate
        );

        this->horizontal_direction = horizontal != Const::DIRECTION::NONE ? horizontal : this->horizontal_direction;
        this->vertical_direction = vertical != Const::DIRECTION::NONE ? vertical : this->vertical_direction;

        this->velocity.x = horizontal != Const::DIRECTION::NONE ? behavior_config.get_velosity() + bonus_velosity : 0;
        this->accelarate.x = horizontal != Const::DIRECTION::NONE ? behavior_config.get_accelarate() : 0;

        this->velocity.y = vertical != Const::DIRECTION::NONE ? behavior_config.get_velosity() + bonus_velosity : 0;
        this->accelarate.y = vertical != Const::DIRECTION::NONE ? behavior_config.get_accelarate() : 0;
    }

    void Character_Node::start_attack(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const Character_Behavior_Config& behavior_config = data->get_config().get_character_behavior_config();

        float animation_duration = this->character_animation->get_amimation_duration("SHOOT");
        this->character_animation->play_animation(
            "SHOOT", behavior_config.get_attack_duration() / animation_duration, true
        );

        this->velocity = {0.f, 0.f};
        this->accelarate = {0.f, 0.f};

        /**
         * Add bullet to map when character fire!
         */
        Battle_Layer* battle_layer = data->get_battle_layer();
        if (!battle_layer)
            return;

        Player_Data& player_data = data->get_player_data();
        Bullet_Node* bullet = new Bullet_Node(
            player_data.get_character_animation_id(), this->horizontal_direction, player_data.get_player_damage()
        );

        float sign_x = horizontal_direction == Const::DIRECTION::LEFT ? -1 : 1;
        glm::vec2 bullet_delta_position = {0, 0};
        if (DELTA_POSITION_BULLET_BY_CHARACTER_ID.find(player_data.get_character_animation_id()) ==
            DELTA_POSITION_BULLET_BY_CHARACTER_ID.end()) {
            bullet_delta_position = DELTA_POSITION_BULLET_BY_CHARACTER_ID[0];
        } else {
            bullet_delta_position = DELTA_POSITION_BULLET_BY_CHARACTER_ID[player_data.get_character_animation_id()];
        }

        glm::vec2 fire_position = this->get_position() + glm::vec2{sign_x, 1} * bullet_delta_position;
        bullet->set_position(fire_position);
        battle_layer->add_child(bullet);

        Shoot_Animation* shoot_animation = new Shoot_Animation(fire_position, this->horizontal_direction);
        battle_layer->remove_child_by_tag(Const::SHOOT_ANIMATION_NODE_TAG);
        battle_layer->add_child(shoot_animation);
    }

    void Character_Node::start_hitted(void* global_data, int source_call_state) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const Character_Behavior_Config& behavior_config = data->get_config().get_character_behavior_config();

        switch (source_call_state) {
            case Const::CHARACTER_ATTACKED_FROM_EMEMY: {
                Character_Collision_Component* collision =
                    Utils::get_component<Character_Collision_Component>(this, Defined::COMPONENT_COLLISION_NAME);
                if (collision == nullptr) {
                    break;
                }
                Player_Data& player_data = data->get_player_data();
                glm::vec2 enemy_direction = collision->get_enemy_direction();
                this->action_character_hitted(0, behavior_config.get_attacked_duration(), enemy_direction);
                this->horizontal_direction = enemy_direction.x >= 0 ? Const::DIRECTION::LEFT : Const::DIRECTION::RIGHT;
                float current_health = player_data.get_current_health() - collision->get_damage_deal();
                player_data.set_current_health(current_health);
                break;
            }
            default: {
                break;
            }
        }
    }

    void Character_Node::start_invincible(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const Character_Behavior_Config& behavior_config = data->get_config().get_character_behavior_config();
        this->action_character_invincible(0, behavior_config.get_invincible_duration());
    }

    void Character_Node::start_dead(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);

        const Character_Behavior_Config& behavior_config = data->get_config().get_character_behavior_config();
        this->remove_component(Defined::COMPONENT_COLLISION_NAME);

        this->action_character_dead(0, behavior_config.get_dead_duration());
    }

    void Character_Node::start_dash(void* global_data, int source_call_state) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        switch (source_call_state) {
            case Const::MIX_DASH_WITH_MOVE: {
                Character_Key_Input_Component* input_component =
                    Utils::get_component<Character_Key_Input_Component>(this, Defined::COMPONENT_KEY_INPUT_NAME);
                if (input_component == nullptr) {
                    break;
                }

                Const::DIRECTION direction = Const::DIRECTION::NONE;
                switch (input_component->get_key_need_handled()) {
                    case Custom::Key::W: {
                        direction = Const::DIRECTION::UP;
                        break;
                    }
                    case Custom::Key::S: {
                        direction = Const::DIRECTION::DOWN;
                        break;
                    }
                    default: {
                        break;
                    }
                }

                const Character_Behavior_Config& behavior_config = data->get_config().get_character_behavior_config();
                this->vertical_direction = direction;
                this->velocity.y = behavior_config.get_velosity();
                this->accelarate.y = behavior_config.get_accelarate();
                break;
            }
            default: {
                const Character_Skill_Dash_Config& dash_skill_config =
                    data->get_config().get_character_skill_dash_config();

                Character_Skill_Dash_Component* dash_skill = Utils::get_component<Character_Skill_Dash_Component>(
                    this, Const::CHARACTER_SKILL_DASH_COMPONENT_NAME
                );

                this->character_animation->play_animation("IDLE");
                this->action_character_dashing(0.f, dash_skill_config.dash_duration, dash_skill_config.dash_distance);

                dash_skill->activating_skill(global_data);
                break;
            }
        }
    }

    void Character_Node::start_channelling(void* global_data, int source_call_state) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);

        switch (source_call_state) {
            case Const::CHARACTER_CHANNLING_FROM_SKILL_THUNDER: {
                const Character_Skill_Thunder_Config& thunder_skill_config =
                    data->get_config().get_character_skill_thunder_config();
                Character_Skill_Thunder_Component* thunder_skill =
                    Utils::get_component<Character_Skill_Thunder_Component>(
                        this, Const::CHARACTER_SKILL_THUNDER_COMPONENT_NAME
                    );
                this->action_character_channelling_skill_thunder(0.f, thunder_skill_config.channelling_duration);
                thunder_skill->activating_skill(global_data);
                break;
            }
            default: {
                break;
            }
        }
    }

    void Character_Node::start_flight(void* global_data, int source_call_state) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const Boss_Skill_Flash_Config& skill_config = data->get_config().get_boss_skill_flash_config();

        this->character_animation->play_animation("IDLE");

        switch (source_call_state) {
            case Const::CHARACTER_FLIGHT_FROM_BOSS_SKILL_FLASH: {
                const Boss_Skill_Flash_Config& skill_config = data->get_config().get_boss_skill_flash_config();
                Player_Data& player_data = data->get_player_data();
                player_data.set_current_health(player_data.get_current_health() - skill_config.flash_damage);
                this->action_character_flight(0.f, skill_config.fly_duration);
                break;
            }
            default: {
                break;
            }
        }
    }

    void Character_Node::start_stun(void* global_data, int source_call_state) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        this->character_animation->play_animation("IDLE");
        switch (source_call_state) {
            case Const::CHARACTER_STUN_FROM_BOSS_SKILL_FLASH: {
                const Boss_Skill_Flash_Config& skill_config = data->get_config().get_boss_skill_flash_config();
                this->action_character_stunned(0.f, skill_config.stun_duration);
                break;
            }
            default: {
                break;
            }
        }
    }

    void Character_Node::end_move(void* global_data, std::string next_state) {
        if (next_state == Const::STATE_DASHING) {
            /**
             * Note: when dashing the velosity is remain make it real.
             */
            this->character_animation->play_animation("IDLE");
        } else {
            this->start_idle();
        }
    }

    void Character_Node::end_attack(void* global_data) {
        this->start_idle();
    }

    void Character_Node::end_hitted(void* global_data, int source_call_state) {
        switch (source_call_state) {
            case Const::CHARACTER_ATTACKED_FROM_EMEMY: {
                this->container->stop_action(ACTION_HITTED_TAG);
                this->attacked_image->stop_action(ACTION_HITTED_TAG);
                this->attacked_image->set_visible(false);
                Utils::reset_to_origin(this->container);
                Utils::reset_to_origin(this->attacked_image);
                break;
            }
            default: {
                break;
            }
        }
    }

    void Character_Node::end_invincible(void* global_data) {
        this->container->stop_action(ACTION_INVINCIBLE_TAG);
        Utils::reset_to_origin(this->container);
    }

    void Character_Node::end_dead(void* global_data) {
        this->container->stop_action(ACTION_DEAD_TAG);
        Utils::reset_to_origin(this->container);
        this->set_visible(false);
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        Player_Data& player_data = data->get_player_data();
        player_data.set_dead(true);
    }

    void Character_Node::end_dash(void* global_data, int source_call_state) {
        this->stop_action(ACTION_DASHING_TAG);
        this->container->stop_action(ACTION_DASHING_TAG);
        Utils::reset_to_origin(this->container);
        this->vertical_direction = Const::DIRECTION::NONE;
        this->velocity = {0.f, 0.f};
        this->accelarate = {0.f, 0.f};
    }

    void Character_Node::end_channelling(void* global_data, int source_call_state) {}

    void Character_Node::end_flight(void* global_data, int source_call_state) {
        switch (source_call_state) {
            case Const::CHARACTER_FLIGHT_FROM_BOSS_SKILL_FLASH: {
                this->container->stop_action(ACTION_FLIGHT_TAG);
                this->attacked_image->stop_action(ACTION_FLIGHT_TAG);
                this->attacked_image->set_visible(false);
                Utils::reset_to_origin(this->container);
                Utils::reset_to_origin(this->attacked_image);
                break;
            }
            default: {
                break;
            }
        }
    }

    void Character_Node::end_stun(void* global_data, int source_call_state) {
        switch (source_call_state) {
            case Const::CHARACTER_STUN_FROM_BOSS_SKILL_FLASH: {
                this->container->stop_action(ACTION_STUN_TAG);
                Utils::reset_to_origin(this->container);
                break;
            }
            default: {
                break;
            }
        }
    }

    void Character_Node::action_character_hitted(float delay, float duration_hitted, glm::vec2 enemy_direction) {
        this->container->stop_action(ACTION_HITTED_TAG);
        Utils::reset_to_origin(this->container);
        float sign = enemy_direction.x > 0 ? -1 : 1;
        glm::vec2 delta_position = glm::vec2{50 * enemy_direction.x, 10};
        this->container->do_action(
            Action::sequence(
                Action::delay(delay),
                Action::spawn(
                    Action::sequence(
                        Action::rotate_to(
                            duration_hitted / 2, -sign * Math::random_float(20, 30), Action_Ease::SINE_OUT
                        ),
                        Action::rotate_to(duration_hitted / 2, 0, Action_Ease::SINE_IN)
                    ),
                    Action::sequence(
                        Action::move_to(duration_hitted / 2, delta_position, Action_Ease::SINE_OUT),
                        Action::move_to(duration_hitted / 2, {0.f, 0.f}, Action_Ease::SINE_IN)
                    ),
                    Action::sequence(
                        Action::scale_to(duration_hitted / 2, {0.9f, 0.9f}, Action_Ease::SINE_OUT),
                        Action::scale_to(duration_hitted / 2, {1.f, 1.f}, Action_Ease::SINE_IN)
                    )
                )
            ),
            ACTION_HITTED_TAG
        );

        this->attacked_image->stop_action(ACTION_HITTED_TAG);
        Utils::reset_to_origin(this->attacked_image);
        this->attacked_image->do_action(
            Action::sequence(Action::delay(delay), Action::show(), Action::fade_out(duration_hitted), Action::hide()),
            ACTION_HITTED_TAG
        );
    }

    void Character_Node::action_character_invincible(float delay, float duration) {
        float duration_fade = duration / NUMBER_FADE_IN_INVINCIBLE_STATE;
        this->container->stop_action(ACTION_INVINCIBLE_TAG);
        Base_Action* action = Action::spawn(
            Action::sequence(
                Action::fade_to(duration_fade / 2, INVISIBLE_OPACITY, Action_Ease::SINE_OUT),
                Action::fade_in(duration_fade / 2, Action_Ease::SINE_IN)
            )
        );
        this->container->do_action(
            Action::sequence(Action::delay(delay), action->repeat(NUMBER_FADE_IN_INVINCIBLE_STATE)),
            ACTION_INVINCIBLE_TAG
        );
    }

    void Character_Node::action_character_dead(float delay, float dead_duration) {
        // this->container->stop_action(ACTION_HITTED_TAG);
        // this->container->stop_action(ACTION_INVINCIBLE_TAG);
        Utils::reset_to_origin(this->container);
        Custom::Transform origin = Utils::get_transform_origin(this->container);
        glm::vec2 start_position = origin.position;
        glm::vec2 end_position = glm::vec2{Math::random_float(-80, 80), 200 + Math::random_float(0, 30)};
        glm::vec2 middle_position = glm::vec2{start_position.x, end_position.y};
        this->set_z_order(1);
        float sign_rotation = Math::random_float() >= 0.5 ? -1 : 1;
        this->container->do_action(
            Action::sequence(
                Action::delay(delay),
                Action::show(),
                Action::spawn(
                    Action::bezier_to(dead_duration, middle_position, end_position, Action_Ease::SINE_IN),
                    Action::sequence(
                        Action::scale_to(dead_duration / 2, {1.2f, 1.2f}, Action_Ease::SINE_OUT),
                        Action::scale_to(dead_duration / 2, {1.f, 1.f}, Action_Ease::SINE_IN)
                    ),
                    Action::rotate_to(
                        dead_duration, sign_rotation * (100.f + Math::random_float(0, 30)), Action_Ease::SINE_IN
                    ),
                    Action::sequence(
                        Action::delay(dead_duration / 2), Action::fade_to(dead_duration / 2, 200, Action_Ease::SINE_OUT)
                    )
                ),
                Action::hide(),
                Action::call_func([this](Base_Node* target, void* global_data) {
                    this->set_visible(false);
                    Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
                    Player_Data& player_data = data->get_player_data();
                    player_data.set_dead(true);
                })
            )
        );
    }

    void Character_Node::action_character_level_up(float delay) {
        Level_Up_Flame_Animation* animation = new Level_Up_Flame_Animation();
        animation->set_position(LEVEL_UP_POSITION);
        this->add_child(animation);

        Label_Node* label_level_up = new Label_Node("LEVEL UP!", "", 40);
        label_level_up->set_color({20, 200, 20});
        this->add_child(label_level_up);

        float duration = 0.5f;
        float duration_fade = 0.35f;
        label_level_up->set_opacity(0);
        label_level_up->set_anchor({0.5, 0.5});
        Base_Action* idle = Action::spawn(
            Action::sequence(Action::fade_to(duration_fade / 2, 120), Action::fade_in(duration_fade / 2)),
            Action::sequence(
                Action::scale_to(duration_fade / 2, {1.05f, 1.05f}, Action_Ease::SINE_OUT),
                Action::scale_to(duration_fade / 2, {1.f, 1.f}, Action_Ease::SINE_IN)
            )
        );
        label_level_up->do_action(
            Action::sequence(
                Action::delay(delay),
                Action::spawn(
                    Action::sequence(
                        Action::move_to(duration / 2, {0, 90}, Action_Ease::SINE_OUT),
                        Action::move_to(duration / 2, {0, 70}, Action_Ease::SINE_IN)
                    ),
                    Action::sequence(
                        Action::scale_to(duration / 2, {1.2f, 1.2f}, Action_Ease::SINE_OUT),
                        Action::scale_to(duration / 2, {1.f, 1.f}, Action_Ease::SINE_IN)
                    ),
                    Action::fade_in(duration * 0.35, Action_Ease::SINE_OUT)
                ),
                idle->repeat(3),
                Action::fade_out(duration / 2),
                Action::remove_self(true)
            )
        );
    }

    void Character_Node::action_character_dashing(float delay, float dash_duration, float dash_distance) {
        this->container->stop_action(ACTION_DASHING_TAG);
        this->stop_action(ACTION_DASHING_TAG);

        float sign = this->horizontal_direction == Const::DIRECTION::LEFT ? -1 : 1;
        float dashing_more = sign * dash_distance;
        this->do_action(
            Action::sequence(
                Action::delay(delay),
                Action::move_to(dash_duration, this->get_position() + glm::vec2(dashing_more, 0), Action_Ease::SINE_IN)
            ),
            ACTION_DASHING_TAG
        );

        Utils::reset_to_origin(this->container);
        int number_rotation = 1;
        float duration_roration = dash_duration / number_rotation;
        this->container->do_action(
            Action::sequence(
                Action::delay(delay),
                Action::spawn(
                    Action::sequence(
                        Action::scale_to(dash_duration / 2, {0.9f, 0.9f}, Action_Ease::SINE_OUT),
                        Action::scale_to(dash_duration / 2, {1.f, 1.f}, Action_Ease::SINE_IN)
                    ),
                    Action::sequence(
                        Action::fade_to(dash_duration / 2, 220, Action_Ease::SINE_OUT),
                        Action::fade_in(dash_duration / 2, Action_Ease::SINE_IN)
                    ),
                    Action::rotate_to(duration_roration, 360 * sign, Action_Ease::SINE_IN)->repeat(number_rotation)
                )
            ),
            ACTION_DASHING_TAG
        );
    }

    void Character_Node::action_character_channelling_skill_thunder(float delay, float duration) {
        Character_Fire_Ball_Animation* animation = new Character_Fire_Ball_Animation();
        this->add_child(animation);
        animation->set_opacity(0);
        animation->set_scale({0.f, 0.f});
        animation->do_action(
            Action::sequence(
                Action::delay(0.f),
                Action::spawn(
                    Action::sequence(
                        Action::scale_to(duration / 2, {1.2f, 1.2f}, Action_Ease::SINE_OUT),
                        Action::scale_to(duration / 2, {1.f, 1.f}, Action_Ease::SINE_IN)
                    ),
                    Action::fade_to(duration * 0.5, ORIGIN_OPACITY_FIRE_BALL_ANIMATION, Action_Ease::SINE_OUT)
                ),
                Action::fade_out(duration * 0.5, Action_Ease::SINE_IN),
                Action::remove_self(true)
            )
        );
    }

    void Character_Node::action_character_flight(float delay, float duration_fly) {
        this->container->stop_action(ACTION_FLIGHT_TAG);
        Utils::reset_to_origin(this->container);
        this->container->do_action(
            Action::sequence(
                Action::delay(delay),
                Action::spawn(
                    Action::sequence(
                        Action::move_to(duration_fly / 2, {0, 200}, Action_Ease::SINE_OUT),
                        Action::move_to(duration_fly / 2, {0, 0}, Action_Ease::SINE_IN)
                    ),
                    Action::rotate_to(duration_fly, 360, Action_Ease::SINE_IN),
                    Action::sequence(
                        Action::fade_to(duration_fly / 2, 220, Action_Ease::SINE_OUT),
                        Action::fade_in(duration_fly / 2, Action_Ease::SINE_IN)
                    ),
                    Action::sequence(
                        Action::scale_to(duration_fly / 2, {1.25f, 1.25f}, Action_Ease::SINE_OUT),
                        Action::scale_to(duration_fly / 2, {1.f, 1.f}, Action_Ease::SINE_IN)
                    )
                )
            ),
            ACTION_FLIGHT_TAG
        );
        this->attacked_image->stop_action(ACTION_FLIGHT_TAG);
        Utils::reset_to_origin(this->container);
        this->attacked_image->set_visible(false);
        this->attacked_image->do_action(
            Action::sequence(
                Action::delay(delay),
                Action::show(),
                Action::fade_to(duration_fly * 0.35, ORIGIN_ATTACKED_IMAGE_OPACITY, Action_Ease::SINE_OUT),
                Action::delay(duration_fly * 0.45),
                Action::hide()
            ),
            ACTION_FLIGHT_TAG
        );
    }

    void Character_Node::action_character_stunned(float delay, float duration_stun) {
        this->container->stop_action(ACTION_STUN_TAG);
        const int NUMBER_ROTATION = 3;
        float duration = duration_stun / NUMBER_ROTATION;
        Utils::reset_to_origin(this->container);
        Base_Action* action_stunned = Action::sequence(
            Action::rotate_to(duration / 2, 5, Action_Ease::SINE_OUT),
            Action::move_to(duration / 2, -5, Action_Ease::SINE_IN)
        );
        this->container->do_action(
            Action::sequence(Action::delay(delay), action_stunned->repeat(NUMBER_ROTATION)), ACTION_STUN_TAG
        );
    }

    void Character_Node::handle_level_up(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        if (data->is_character_level_up()) {
            data->character_level_up();
            this->action_character_level_up(0.f);
            Player_Data& player_data = data->get_player_data();
            this->character_animation->set_character_level(player_data.get_character_level());
        }
    }

    void Character_Node::fix_update(float delta_time, void* global_data) {
        Character_State_Machine_Component* state_machine =
            Utils::get_component<Character_State_Machine_Component>(this, Defined::COMPONENT_STATE_MACHINE_NAME);
        if (state_machine->is_character_dead()) {
            return;
        }
        this->handle_level_up(global_data);
        this->update_moverment(delta_time);
        this->update_character_direction();
        this->sync_player_data(global_data);
        this->sync_attacked_image();
        Game_Object::fix_update(delta_time, global_data);
    }
}  // namespace Meow_Meow