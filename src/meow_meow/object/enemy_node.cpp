#include <actions.h>
#include <collision_component.h>
#include <defined.h>
#include <math_custom.h>
#include <meow_meow/component/enemy_behavior_component.h>
#include <meow_meow/const.h>
#include <meow_meow/data/bullet_collision_data.h>
#include <meow_meow/data/enemy_collision_data.h>
#include <meow_meow/data/global_data.h>
#include <meow_meow/object/enemy_node.h>
#include <state_machine_component.h>
#include <utils.h>

namespace Meow_Meow {
    Enemy_Node::Enemy_Node() {
        this->init_container();
        this->init_enemy_animation();
        this->init_components();
        this->init_attacked_image();
        this->init_progression_health();
    }

    Enemy_Node::~Enemy_Node() {
        Collision_Component* collision =
            Utils::get_component<Collision_Component>(this, Defined::COMPONENT_COLLISION_NAME);
        Enemy_Collision_Data* data = Utils::get_collision_owner_data<Enemy_Collision_Data>(collision);
        delete (data);
    }

    Custom::Transformed_Rectangle Enemy_Node::get_bounding_box(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const Character_Behavior_Config& behavior_config = data->get_config().get_character_behavior_config();

        Custom::Size bounding_size = behavior_config.get_bounding_box();
        return Custom::Transformed_Rectangle{
            Custom::Rectangle{bounding_size.width, bounding_size.height}.apply(this->get_transform(), {0.5f, 0.5f})
        };
    }

    void Enemy_Node::handle_boundary(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const Character_Behavior_Config& behavior_config = data->get_config().get_character_behavior_config();

        Battle_Layer* battle_layer = data->get_battle_layer();
        Custom::Size layer_size = battle_layer->get_content_size();
        Custom::Rectangle_Area layer_rect{0, 0, layer_size.width, layer_size.height};
        Custom::Size enemy_bounding_size = behavior_config.get_bounding_box();
        glm::vec2 enemy_start_box_position =
            this->get_position() - glm::vec2{0.5f, 0.5f} * enemy_bounding_size.to_vec2();
        Custom::Rectangle_Area enemy_rect = {
            enemy_start_box_position.x,
            enemy_start_box_position.y,
            enemy_bounding_size.width,
            enemy_bounding_size.height
        };
        Custom::Rectangle_Area enemy_rect_fix_with_layer = enemy_rect;
        enemy_rect.fix_with(layer_rect);
        this->set_position(
            this->get_position() - (enemy_rect_fix_with_layer.get_position() - enemy_rect.get_position())
        );
    }

    void Enemy_Node::init_container() {
        this->container = new Node();
        this->container->set_cascade_opacity(true);
        Utils::save_transform_origin(this->container);
        this->add_child(this->container);
    }

    void Enemy_Node::init_enemy_animation() {
        this->enemy_animation = new Character_Animation(this->enemy_animation_id, 1);
        this->enemy_animation->set_character_id(this->enemy_animation_id);
        this->enemy_animation->set_name("debug");
        this->container->add_child(this->enemy_animation);
    }

    void Enemy_Node::init_attacked_image() {
        this->attacked_image = new Image_UI_Node();
        this->attacked_image->set_force_renderer_color({255, 0, 0});
        this->attacked_image->set_enable_force_renderer_color(true);
        this->attacked_image->set_visible(false);
        this->attacked_image->set_opacity(ORIGIN_ATTACKED_IMAGE_OPACITY);
        Utils::save_transform_origin(this->attacked_image);
        this->container->add_child(this->attacked_image);
    }

    void Enemy_Node::init_components() {
        Enemy_Behavior_Component* behavior = new Enemy_Behavior_Component();
        behavior->set_name(Const::ENEMY_BEHAVIOR_COMPONENT_NAME);
        this->add_component(behavior);

        State_Machine_Component* state_machine = new State_Machine_Component();
        state_machine->add_track(Const::TRACK_CONTROLL, nullptr);
        state_machine->add_track(Const::TRACK_EFFECTED, nullptr);
        state_machine->set_name(Defined::COMPONENT_STATE_MACHINE_NAME);
        this->add_component(state_machine);

        Collision_Component* collision = new Collision_Component();
        collision->set_name(Defined::COMPONENT_COLLISION_NAME);
        collision->set_tag(Const::ENEMY_COLLISION_TAG);
        collision->set_box_size(Const::ENEMY_BOUNDING_BOX);
        collision->set_track_layer(Const::BATTLE_LAYER_COLLISION);
        collision->set_owner_data(new Enemy_Collision_Data());
        this->add_component(collision);

        this->add_key_press_listener(Custom::Key::V);
    }

    void Enemy_Node::init_progression_health() {
        this->progression_health = Progression_Node::make(
            Const::PATH_HEALTH_BAR,
            Const::HEALTH_BAR_CAP_INSETS,
            Const::HEALTH_BAR_SIZE,
            Const::HEALTH_BAR_COLOR,
            Const::HEALTH_BAR_PADDING,
            Const::HEALTH_BAR_DELTA_POSITION
        );
        this->progression_health->set_position(ORIGIN_HEALTH_BAR_POSITION);
        this->progression_health->set_percent(50);
        this->progression_health->set_visible(false);
        this->add_child(this->progression_health);
        Utils::save_transform_origin(this->progression_health);
    }

    void Enemy_Node::attach(void* global_data) {
        this->enemy_animation->play_animation("IDLE");
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);

        const Enemy_Behavior_Config& behavior_config = data->get_config().get_enemy_behavior_config();
        this->current_health = behavior_config.get_enemy_health();
        this->progression_health->set_percent(100);

        Collision_Component* collision =
            Utils::get_component<Collision_Component>(this, Defined::COMPONENT_COLLISION_NAME);
        collision->set_box_size(behavior_config.get_bounding_box());
    }

    void Enemy_Node::change_to_attack(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const auto& behavior_config = data->get_config().get_enemy_behavior_config();

        State_Machine_Component* state_machine =
            Utils::get_component<State_Machine_Component>(this, Defined::COMPONENT_STATE_MACHINE_NAME);

        Enemy_Behavior_Component* behavior =
            Utils::get_component<Enemy_Behavior_Component>(this, Const::ENEMY_BEHAVIOR_COMPONENT_NAME);

        Collision_Component* collision =
            Utils::get_component<Collision_Component>(this, Defined::COMPONENT_COLLISION_NAME);

        behavior->start_attack_countdown();
        float animation_duration = this->enemy_animation->get_amimation_duration("ATTACK");
        this->enemy_animation->play_animation(
            "ATTACK", behavior_config.get_enemy_attack_duration() / animation_duration
        );

        state_machine->change_state_at(
            Const::TRACK_CONTROLL, Const::STATE_ATTACK, behavior_config.get_enemy_attack_duration()
        );

        /**
         * Attach the damage into collision data
         */
        Enemy_Collision_Data* collision_data = Utils::get_collision_owner_data<Enemy_Collision_Data>(collision);
        collision_data->set_damage_deal(behavior_config.get_emeny_attack_damage());
    }

    void Enemy_Node::change_to_death(void* global_data) {
        this->enemy_animation->play_animation("DEAD");
        Utils::reset_to_origin(this->progression_health);
        this->progression_health->stop_action(HITTED_ACTION_TAG);
        this->progression_health->do_action(Action::sequence(Action::delay(0), Action::fade_out(0.2), Action::hide()));
        this->enemy_animation->on_finish_animation_callback("DEAD", [this](Animation_Node*, void*) {
            this->remove_from_parent();
        });
    }

    void Enemy_Node::change_to_hitted(void* global_data, float damage_take, Const::DIRECTION bullet_direction) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const Enemy_Behavior_Config& behavior_config = data->get_config().get_enemy_behavior_config();

        State_Machine_Component* state_machine =
            Utils::get_component<State_Machine_Component>(this, Defined::COMPONENT_STATE_MACHINE_NAME);

        Enemy_Behavior_Component* behavior =
            Utils::get_component<Enemy_Behavior_Component>(this, Const::ENEMY_BEHAVIOR_COMPONENT_NAME);

        if (state_machine->get_current_state_at(Const::TRACK_CONTROLL) == Const::STATE_JUMP) {
            this->stop_action(JUMP_ACTION_TAG);
            this->container->stop_action(JUMP_ACTION_TAG);
            Utils::reset_to_origin(this->container);
        }

        float max_health = behavior_config.get_enemy_health();
        this->current_health = std::max(this->current_health - damage_take, 0.f);
        float percent = (this->current_health / max_health) * 100;

        this->action_enemy_hitted(0, behavior_config.get_enemy_attacked_duration(), bullet_direction, percent);

        if (current_health <= 0) {
            state_machine->change_state_at(
                Const::TRACK_CONTROLL, Const::STATE_DEATH, State_Machine_Component::INFITY_STATE
            );
            this->velosity = {0, 0};
        } else {
            this->velosity = -behavior_config.get_enemy_attacked_velosity() * behavior->get_enemy_walking_direction();
        }

        state_machine->change_state_at(
            Const::TRACK_EFFECTED, Const::STATE_ATTACKED, behavior_config.get_enemy_attacked_duration()
        );

        float duration = this->enemy_animation->get_amimation_duration("IDLE");
        this->enemy_animation->play_animation("IDLE", behavior_config.get_enemy_attacked_duration() / duration);
    }

    void Enemy_Node::change_to_jump(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const auto& behavior_config = data->get_config().get_enemy_behavior_config();

        State_Machine_Component* state_machine =
            Utils::get_component<State_Machine_Component>(this, Defined::COMPONENT_STATE_MACHINE_NAME);

        Enemy_Behavior_Component* behavior =
            Utils::get_component<Enemy_Behavior_Component>(this, Const::ENEMY_BEHAVIOR_COMPONENT_NAME);

        behavior->start_jump_countdown();
        float animation_duration = this->enemy_animation->get_amimation_duration("IDLE");
        this->enemy_animation->play_animation("IDLE", behavior_config.get_enemy_jump_duration() / animation_duration);
        this->action_enemy_jump(0.f, behavior_config.get_enemy_jump_duration(), behavior->get_jump_position());

        state_machine->change_state_at(
            Const::TRACK_CONTROLL, Const::STATE_JUMP, behavior_config.get_enemy_jump_duration()
        );
    }

    void Enemy_Node::change_to_walk(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const auto& behavior_config = data->get_config().get_enemy_behavior_config();

        State_Machine_Component* state_machine =
            Utils::get_component<State_Machine_Component>(this, Defined::COMPONENT_STATE_MACHINE_NAME);

        Enemy_Behavior_Component* behavior =
            Utils::get_component<Enemy_Behavior_Component>(this, Const::ENEMY_BEHAVIOR_COMPONENT_NAME);

        this->velosity = behavior_config.get_enemy_velosity() * behavior->get_enemy_walking_direction();

        state_machine->change_state_at(
            Const::TRACK_CONTROLL, Const::STATE_WALK, behavior_config.get_enemy_walk_duration()
        );

        std::string last_state = state_machine->get_last_state_processign_at(Const::TRACK_CONTROLL);
        if (last_state != Const::STATE_WALK) {
            float duration = this->enemy_animation->get_amimation_duration("WALK");
            this->enemy_animation->play_animation("WALK", behavior_config.get_enemy_walk_duration() / duration);
        }
    }

    void Enemy_Node::action_enemy_jump(float delay, float duration, glm::vec2 character_position) {
        this->stop_action(JUMP_ACTION_TAG);
        glm::vec2 start_position = this->get_position();
        glm::vec2 end_position = character_position;
        glm::vec2 delta_position = end_position - start_position;
        glm::vec2 middle_position = Math::get_middle_bezier_point(
            start_position, end_position, 200 + Math::random_float(0, 50), 0.5f, delta_position.x > 0 ? -1 : 1
        );
        float sign_rotation = delta_position.x > 0 ? -1 : 1;
        this->do_action(
            Action::sequence(
                Action::delay(delay), Action::bezier_to(duration, middle_position, end_position, Action_Ease::SINE_IN)
            ),
            JUMP_ACTION_TAG
        );
        this->container->do_action(
            Action::sequence(
                Action::delay(delay),
                Action::spawn(
                    Action::sequence(
                        Action::scale_to(duration / 2, {0.85, 0.85}, Action_Ease::SINE_OUT),
                        Action::scale_to(duration / 2, {1.0, 1.0}, Action_Ease::SINE_IN)
                    ),
                    Action::sequence(
                        Action::rotate_to(duration / 2, sign_rotation * 30, Action_Ease::SINE_OUT),
                        Action::rotate_to(duration / 2, 0, Action_Ease::SINE_IN)
                    )
                )
            ),
            JUMP_ACTION_TAG
        );
    }

    void Enemy_Node::action_enemy_hitted(
        float delay, float duration, Const::DIRECTION bullet_direction, float percent_health
    ) {
        this->container->stop_action(HITTED_ACTION_TAG);
        float sign = bullet_direction == Const::DIRECTION::LEFT ? -1 : 1;
        this->container->do_action(
            Action::sequence(
                Action::delay(delay),
                Action::spawn(
                    Action::sequence(
                        Action::rotate_to(duration / 2, Math::random_float(25, 30) * sign, Action_Ease::SINE_OUT),
                        Action::rotate_to(duration / 2, 0, Action_Ease::SINE_IN)
                    ),
                    Action::sequence(
                        Action::scale_to(duration / 2, {1.1f, 1.1f}, Action_Ease::SINE_OUT),
                        Action::scale_to(duration / 2, {1.f, 1.f}, Action_Ease::SINE_IN)
                    ),
                    Action::sequence(
                        Action::fade_to(duration / 2, 220, Action_Ease::SINE_OUT),
                        Action::fade_in(duration / 2, Action_Ease::SINE_IN)
                    ),
                    Action::sequence(
                        Action::move_to(duration / 2, {10 * sign, 0}, Action_Ease::SINE_OUT),
                        Action::move_to(duration / 2, {0, 0}, Action_Ease::SINE_IN)
                    )
                )
            ),
            HITTED_ACTION_TAG
        );
        this->attacked_image->stop_action(HITTED_ACTION_TAG);
        Utils::reset_to_origin(this->attacked_image);
        this->attacked_image->do_action(
            Action::sequence(
                Action::delay(delay), Action::show(), Action::fade_out(duration, Action_Ease::SINE_IN), Action::hide()
            ),
            HITTED_ACTION_TAG
        );

        Custom::Transform origin = Utils::get_transform_origin(this->progression_health);
        this->progression_health->stop_action(HITTED_ACTION_TAG);
        if (this->progression_health->is_visible()) {
            Utils::reset_to_origin(this->progression_health);
            this->progression_health->set_visible(true);
            this->progression_health->do_action(
                Action::sequence(
                    Action::spawn(
                        Action::delay(DURATION_SHOW_PROGRESSION_HEALTH),
                        Action::progress_to(duration, percent_health, Action_Ease::SINE_IN)
                    ),
                    Action::spawn(
                        Action::sequence(
                            Action::move_to(duration / 2, origin.position + glm::vec2(0, 20), Action_Ease::SINE_OUT),
                            Action::move_to(duration / 2, origin.position - glm::vec2(0, 50), Action_Ease::SINE_IN)
                        ),
                        Action::fade_out(duration / 2)
                    ),
                    Action::hide()
                ),
                HITTED_ACTION_TAG
            );
        } else {
            Utils::reset_to_origin(this->progression_health);
            glm::vec2 start_position = origin.position - glm::vec2{0, -50};
            this->progression_health->set_position(start_position);
            this->progression_health->set_opacity(0);
            this->progression_health->do_action(
                Action::sequence(
                    Action::delay(delay),
                    Action::show(),
                    Action::spawn(
                        Action::sequence(
                            Action::move_to(duration / 2, origin.position + glm::vec2(0, 20), Action_Ease::SINE_OUT),
                            Action::move_to(duration / 2, origin.position, Action_Ease::SINE_IN)
                        ),
                        Action::fade_in(duration * 0.35, Action_Ease::SINE_OUT),
                        Action::progress_to(duration, percent_health, Action_Ease::SINE_IN),
                        Action::delay(DURATION_SHOW_PROGRESSION_HEALTH)
                    ),
                    Action::spawn(
                        Action::sequence(
                            Action::move_to(duration / 2, origin.position + glm::vec2(0, 20), Action_Ease::SINE_OUT),
                            Action::move_to(duration / 2, origin.position - glm::vec2(0, 50), Action_Ease::SINE_IN)
                        ),
                        Action::fade_out(duration / 2)
                    ),
                    Action::hide()
                ),
                HITTED_ACTION_TAG
            );
        }
    }

    void Enemy_Node::handle_collision(float delta_time, void* global_data) {
        Collision_Component* collision_component =
            Utils::get_component<Collision_Component>(this, Defined::COMPONENT_COLLISION_NAME);

        State_Machine_Component* state_machine =
            Utils::get_component<State_Machine_Component>(this, Defined::COMPONENT_STATE_MACHINE_NAME);

        if (!state_machine->is_finish_state_at(Const::TRACK_EFFECTED)) {
            std::string current_state = state_machine->get_current_state_at(Const::TRACK_EFFECTED);
            if (current_state == Const::STATE_ATTACKED) {
                return;
            }
        }

        std::vector<Collision_Information> collisions = collision_component->get_collisioneds();
        for (Collision_Information& collision : collisions) {
            if (collision.tag != Const::BULLET_COLLISION_TAG)
                continue;
            Bullet_Collision_Data* collision_data = reinterpret_cast<Bullet_Collision_Data*>(collision.owner_data);
            if (collision_data->get_damage_deal() <= 0 || collision_data->is_hitted())
                continue;
            this->change_to_hitted(
                global_data, collision_data->get_damage_deal(), collision_data->get_bullet_direction()
            );
            collision_data->set_damage_deal(0.f);
            collision_data->set_hitted(true);
            break;
        }
    }

    void Enemy_Node::handle_state_machine(float delta_time, void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const auto& behavior_config = data->get_config().get_enemy_behavior_config();

        State_Machine_Component* state_machine =
            Utils::get_component<State_Machine_Component>(this, Defined::COMPONENT_STATE_MACHINE_NAME);

        Enemy_Behavior_Component* behavior =
            Utils::get_component<Enemy_Behavior_Component>(this, Const::ENEMY_BEHAVIOR_COMPONENT_NAME);

        Collision_Component* collision =
            Utils::get_component<Collision_Component>(this, Defined::COMPONENT_COLLISION_NAME);

        if (state_machine->get_current_state_at(Const::TRACK_CONTROLL) == "") {
            this->change_to_walk(global_data);
            state_machine->change_state_at(
                Const::TRACK_EFFECTED, Const::STATE_UNEFFECTED, State_Machine_Component::INFITY_STATE
            );
            return;
        }

        if (state_machine->is_finish_state_at(Const::TRACK_CONTROLL)) {
            /**
             * @Note: if enemy is attacked we don't update anything!
             */
            if (!(state_machine->get_current_state_at(Const::TRACK_EFFECTED) == Const::STATE_ATTACKED &&
                  !state_machine->is_finish_state_at(Const::TRACK_CONTROLL))) {
                std::string current_state = state_machine->get_current_state_at(Const::TRACK_CONTROLL);
                if (current_state != Const::STATE_DEATH) {
                    /**
                     * If duration attack finish and player is not take it
                     * we remove the damage deal in collision!
                     */
                    if (current_state == Const::STATE_ATTACK) {
                        Enemy_Collision_Data* collision_data =
                            Utils::get_collision_owner_data<Enemy_Collision_Data>(collision);
                        collision_data->set_damage_deal(0.f);
                    }

                    if (behavior->can_attack()) {
                        this->change_to_attack(global_data);
                    } else if (behavior->can_jump()) {
                        this->change_to_jump(global_data);
                    } else {
                        this->change_to_walk(global_data);
                    }
                }
            }
        }

        if (state_machine->is_finish_state_at(Const::TRACK_EFFECTED)) {
            std::string current_state = state_machine->get_current_state_at(Const::TRACK_EFFECTED);
            if (current_state == Const::STATE_ATTACKED) {
                state_machine->change_state_at(
                    Const::TRACK_EFFECTED, Const::STATE_UNEFFECTED, State_Machine_Component::INFITY_STATE
                );
                this->container->stop_action(HITTED_ACTION_TAG);
                Utils::reset_to_origin(this->container);
                std::string current_controll_state = state_machine->get_current_state_at(Const::TRACK_CONTROLL);
                if (current_controll_state == Const::STATE_WALK) {
                    float duration = this->enemy_animation->get_amimation_duration("WALK");
                    this->enemy_animation->play_animation("WALK", behavior_config.get_enemy_walk_duration() / duration);
                } else if (current_controll_state == Const::STATE_DEATH) {
                    this->change_to_death(global_data);
                }
            }
        }
    }

    void Enemy_Node::update_movement(float delta_time) {
        this->set_position(this->get_position() + this->velosity * delta_time);
    }

    void Enemy_Node::update_enemy_direction() {
        Enemy_Behavior_Component* behavior =
            Utils::get_component<Enemy_Behavior_Component>(this, Const::ENEMY_BEHAVIOR_COMPONENT_NAME);

        State_Machine_Component* state_machine =
            Utils::get_component<State_Machine_Component>(this, Defined::COMPONENT_STATE_MACHINE_NAME);

        if (state_machine->get_current_state_at(Const::TRACK_CONTROLL) == Const::STATE_DEATH) {
            return;
        }

        bool is_right = behavior->get_enemy_walking_direction().x > 0;
        this->enemy_animation->set_flipped_x(is_right);
        glm::vec2 anchor = !is_right
                               ? ORIGIN_ANIMATION_ANCHOR_POINT.to_vec2()
                               : glm::vec2{1.0f - ORIGIN_ANIMATION_ANCHOR_POINT.x, ORIGIN_ANIMATION_ANCHOR_POINT.y};
        this->enemy_animation->set_anchor(anchor);
    }

    void Enemy_Node::on_key_pressed(Custom::Key key, Key_Press_Detail pressed_detail, void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const auto& behavior_config = data->get_config().get_enemy_behavior_config();

        Enemy_Behavior_Component* behavior =
            Utils::get_component<Enemy_Behavior_Component>(this, Const::ENEMY_BEHAVIOR_COMPONENT_NAME);

        switch (key) {
            case Custom::V: {
                if (pressed_detail.type == Key_Input_Type::PRESSED) {
                    glm::vec2 direction = behavior->get_enemy_walking_direction();
                    glm::vec2 jump_position = direction * behavior_config.get_enemy_jump_distane();
                    this->action_enemy_jump(0, behavior_config.get_enemy_jump_duration(), jump_position);
                }
                break;
            }
            default: {
                break;
            }
        }
    }

    void Enemy_Node::sync_attacked_image() {
        if (!this->attacked_image->is_visible()) {
            return;
        }
        this->attacked_image->set_image(this->enemy_animation->get_image());
        this->attacked_image->set_anchor(this->enemy_animation->get_anchor().to_vec2());
        this->attacked_image->set_flipped_x(this->enemy_animation->is_flipped_x());
    }

    void Enemy_Node::update_colision_data() {
        Enemy_Behavior_Component* behavior =
            Utils::get_component<Enemy_Behavior_Component>(this, Const::ENEMY_BEHAVIOR_COMPONENT_NAME);
        /**
         * @Note: update direction to collision data to player do effect hitted right away!
         */
        Collision_Component* collision =
            Utils::get_component<Collision_Component>(this, Defined::COMPONENT_COLLISION_NAME);

        Enemy_Collision_Data* collision_data = Utils::get_collision_owner_data<Enemy_Collision_Data>(collision);
        collision_data->set_enemy_direction(behavior->get_enemy_walking_direction());
    }

    void Enemy_Node::fix_update(float delta_time, void* global_data) {
        this->handle_collision(delta_time, global_data);
        this->handle_state_machine(delta_time, global_data);
        this->update_movement(delta_time);
        this->update_enemy_direction();
        this->sync_attacked_image();
        this->update_colision_data();
        Game_Object::fix_update(delta_time, global_data);
    }
}  // namespace Meow_Meow