#include <actions.h>
#include <meow_meow/component/boss_skill_flash_component.h>
#include <meow_meow/data/boss_collision_data.h>
#include <meow_meow/data/global_data.h>
#include <meow_meow/object/boss_node.h>
#include <utils.h>

namespace Meow_Meow {
    Boss_Node::Boss_Node() {}

    Boss_Node::Boss_Node(int enemy_id, int enemy_character_id) : Enemy_Node(enemy_id, enemy_character_id) {
        this->set_name("Boss_Node");
    }

    Boss_Node::~Boss_Node() {}

    const Enemy_Behavior_Config& Boss_Node::get_behavior_config_from(void* global_data) const {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        return data->get_config().get_boss_behavior_config();
    }

    void Boss_Node::update_collision_component(Collision_Component* collision) {
        collision->set_tag(Const::BOSS_COLLISION_TAG);
        collision->set_owner_data(new Boss_Collision_Data());
    }

    void Boss_Node::init_skill_components() {
        Boss_Skill_Flash_Component* flash_skill = new Boss_Skill_Flash_Component();
        flash_skill->set_name(Const::BOSS_SKILL_FLASH_COMPONENT_NAME);
        this->add_component(flash_skill);
    }

    bool Boss_Node::handle_active_skill(void* global_data) {
        State_Machine_Component* state_machine =
            Utils::get_component<State_Machine_Component>(this, Defined::COMPONENT_STATE_MACHINE_NAME);

        Boss_Skill_Flash_Component* flash_skill =
            Utils::get_component<Boss_Skill_Flash_Component>(this, Const::BOSS_SKILL_FLASH_COMPONENT_NAME);

        if (flash_skill->can_activate_skill(state_machine, global_data)) {
            this->change_to_channeling(global_data, flash_skill->get_skill_id());
            return true;
        }

        return false;
    }

    void Boss_Node::remove_from_battle(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        Battle_Layer* battle_layer = data->get_battle_layer();
        if (battle_layer == nullptr)
            return;
        battle_layer->remove_boss_by(this->get_enemy_id());
    }

    void Boss_Node::clean_collision_data(Collision_Component* collision) {
        Boss_Collision_Data* collision_data = Utils::get_collision_owner_data<Boss_Collision_Data>(collision);
        delete (collision_data);
    }

    bool Boss_Node::handle_other_state(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const Boss_Skill_Flash_Config& flash_skill_config = data->get_config().get_boss_skill_flash_config();

        State_Machine_Component* state_machine =
            Utils::get_component<State_Machine_Component>(this, Defined::COMPONENT_STATE_MACHINE_NAME);

        if (state_machine->is_finish_state_at(Const::TRACK_CONTROLL)) {
            std::string current_state = state_machine->get_current_state_at(Const::TRACK_CONTROLL);
            if (current_state == Const::STATE_SKILL_CHANNELLING) {
                this->container->stop_action(ACTION_CHANNELLING_SKILL_TAG);
                this->attacked_image->stop_action(ACTION_CHANNELLING_SKILL_TAG);
                Utils::reset_to_origin(this->container);
                Utils::reset_to_origin(this->attacked_image);
                if (this->channelling_skill == flash_skill_config.skill_id) {
                    this->change_to_flash(global_data);
                    return true;
                }
            }
            if (current_state == Const::STATE_SKILL_FLASH) {
                this->stop_action(ACTION_FLASHING_SKILL_TAG);
                this->container->stop_action(ACTION_FLASHING_SKILL_TAG);
                Utils::reset_to_origin(this->container);
                this->enemy_animation->play_animation("IDLE");
                /**
                 * When end skill but chacter don't get damage we remove it!
                 */
                Collision_Component* collision =
                    Utils::get_component<Collision_Component>(this, Defined::COMPONENT_COLLISION_NAME);
                Boss_Collision_Data* collision_data = Utils::get_collision_owner_data<Boss_Collision_Data>(collision);
                collision_data->set_using_skill_id("");
                collision_data->set_skill_damage(0);
                return false;
            }
        }

        return false;
    }

    void Boss_Node::action_channelling_skill(float delay, float duration_channelling) {
        this->enemy_animation->play_animation("IDLE");
        this->container->stop_action(ACTION_CHANNELLING_SKILL_TAG);
        Utils::reset_to_origin(this->container);
        const int NUMBER_CHANNELLING = 3;
        float duration = duration_channelling / NUMBER_CHANNELLING;
        this->container->do_action(
            Action::sequence(
                Action::delay(delay),
                Action::spawn(
                    Action::scale_to(duration / 2, {1.1f, 1.1f}, Action_Ease::SINE_OUT),
                    Action::scale_to(duration / 2, {1.f, 1.f}, Action_Ease::SINE_IN)
                )
            ),
            ACTION_CHANNELLING_SKILL_TAG
        );
        this->attacked_image->do_action(
            Action::sequence(Action::delay(delay), Action::show(), Action::delay(duration_channelling), Action::hide()),
            ACTION_CHANNELLING_SKILL_TAG
        );
    }

    void Boss_Node::change_to_channeling(void* global_data, std::string skill_id) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const Boss_Skill_Flash_Config& flash_skill_config = data->get_config().get_boss_skill_flash_config();

        State_Machine_Component* state_machine =
            Utils::get_component<State_Machine_Component>(this, Defined::COMPONENT_STATE_MACHINE_NAME);

        this->channelling_skill = skill_id;
        float duration_channelling = 0.f;
        if (skill_id == flash_skill_config.skill_id) {
            duration_channelling = flash_skill_config.duration_channeling;
        }

        state_machine->change_state_at(Const::TRACK_CONTROLL, Const::STATE_SKILL_CHANNELLING, duration_channelling);
        this->action_channelling_skill(0.f, duration_channelling);
    }

    void Boss_Node::action_flashing(float delay, float duration_flash, glm::vec2 flash_position) {
        this->stop_action(ACTION_FLASHING_SKILL_TAG);
        this->container->stop_action(ACTION_FLASHING_SKILL_TAG);
        this->do_action(
            Action::sequence(Action::delay(delay), Action::move_to(duration_flash, flash_position)),
            ACTION_FLASHING_SKILL_TAG
        );
        this->container->do_action(
            Action::sequence(
                Action::delay(delay),
                Action::spawn(
                    Action::sequence(
                        Action::scale_to(duration_flash / 2, {0.9f, 0.9f}, Action_Ease::SINE_OUT),
                        Action::scale_to(duration_flash / 2, {1.f, 1.f}, Action_Ease::SINE_IN)
                    ),
                    Action::sequence(
                        Action::fade_to(duration_flash / 2, 220, Action_Ease::SINE_OUT),
                        Action::fade_in(duration_flash / 2, Action_Ease::SINE_OUT)
                    )
                )
            ),
            ACTION_FLASHING_SKILL_TAG
        );
    }

    void Boss_Node::change_to_flash(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const Boss_Skill_Flash_Config& flash_skill_config = data->get_config().get_boss_skill_flash_config();

        State_Machine_Component* state_machine =
            Utils::get_component<State_Machine_Component>(this, Defined::COMPONENT_STATE_MACHINE_NAME);

        Boss_Skill_Flash_Component* flash_skill =
            Utils::get_component<Boss_Skill_Flash_Component>(this, Const::BOSS_SKILL_FLASH_COMPONENT_NAME);

        Collision_Component* collision =
            Utils::get_component<Collision_Component>(this, Defined::COMPONENT_COLLISION_NAME);
        Boss_Collision_Data* collision_data = Utils::get_collision_owner_data<Boss_Collision_Data>(collision);

        collision_data->set_using_skill_id(flash_skill_config.skill_id);
        collision_data->set_skill_damage(flash_skill_config.flash_damage);

        state_machine->change_state_at(
            Const::TRACK_CONTROLL, Const::STATE_SKILL_FLASH, flash_skill_config.duration_flash
        );
        this->action_flashing(0.f, flash_skill_config.duration_flash, flash_skill->get_flash_to_position());
    }

}  // namespace Meow_Meow