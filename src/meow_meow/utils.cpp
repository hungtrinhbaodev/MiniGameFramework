#include <actions.h>
#include <meow_meow/utils.h>

namespace Meow_Meow {
    void fix_background_to_scene(Image_Node* bg, Custom::Size screen_size) {
        Custom::Size bg_size = bg->get_content_size();
        bg->set_position({screen_size.width / 2, screen_size.height / 2});
        float scale = std::max(screen_size.width / bg_size.width, screen_size.height / bg_size.height);
        bg->set_scale({scale, scale});
    }

    void run_action_idle_logo(Node* logo, glm::vec2 origin_scale) {
        float duration = 0.35;
        Base_Action* logo_action = Action::sequence(
            Action::scale_to(duration / 2, glm::vec2({1.15, 1.15}) * origin_scale, Action_Ease::SINE_OUT),
            Action::scale_to(duration / 2, origin_scale, Action_Ease::SINE_IN),
            Action::rotate_by(duration / 2, -5, Action_Ease::SINE_IN),
            Action::rotate_by(duration / 2, 5, Action_Ease::SINE_OUT)
        );
        logo->do_action(logo_action->repeat_forever());
    }

    Character_Node* cast_charater_target(Base_Node* target) {
        return reinterpret_cast<Character_Node*>(target);
    }

    Character_State_Machine_Component* get_character_state_machine(Base_Node* target) {
        Character_Node* character = cast_charater_target(target);
        return Utils::get_component<Character_State_Machine_Component>(
            character, Defined::COMPONENT_STATE_MACHINE_NAME
        );
    }

    Character_Skill_Thunder_Component* get_skill_thunder_component(Base_Node* target) {
        Character_Node* character = cast_charater_target(target);
        return Utils::get_component<Character_Skill_Thunder_Component>(
            character, Const::CHARACTER_SKILL_THUNDER_COMPONENT_NAME
        );
    }

    Character_Skill_Dash_Component* get_skill_dash_component(Base_Node* target) {
        Character_Node* character = cast_charater_target(target);
        return Utils::get_component<Character_Skill_Dash_Component>(
            character, Const::CHARACTER_SKILL_DASH_COMPONENT_NAME
        );
    }

    Enemy_Node* cast_enemy_target(Base_Node* target) {
        return reinterpret_cast<Enemy_Node*>(target);
    }

    Enemy_State_Machine_Component* get_enemy_state_machine_component(Base_Node* target) {
        Enemy_Node* enemy = cast_enemy_target(target);
        return Utils::get_component<Enemy_State_Machine_Component>(enemy, Defined::COMPONENT_STATE_MACHINE_NAME);
    }

    Enemy_Behavior_Component* get_enemy_bihavior_component(Base_Node* target) {
        Enemy_Node* enemy = cast_enemy_target(target);
        return Utils::get_component<Enemy_Behavior_Component>(enemy, Const::ENEMY_BEHAVIOR_COMPONENT_NAME);
    }

    Boss_Node* cast_boss_target(Base_Node* target) {
        return reinterpret_cast<Boss_Node*>(target);
    }

    Boss_State_Machine_Component* get_boss_state_machine_component(Base_Node* target) {
        Boss_Node* boss = cast_boss_target(target);
        return Utils::get_component<Boss_State_Machine_Component>(boss, Defined::COMPONENT_STATE_MACHINE_NAME);
    }

    Boss_Skill_Throw_Enemy_Component* get_skill_throw_enemy_component(void* global_data, int boss_id) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        Battle_Layer* battle_layer = data->get_battle_layer();
        if (battle_layer == nullptr)
            return nullptr;
        Boss_Node* boss = battle_layer->get_boss_by(boss_id);
        if (boss == nullptr)
            return nullptr;
        Boss_Skill_Throw_Enemy_Component* throw_skill =
            Utils::get_component<Boss_Skill_Throw_Enemy_Component>(boss, Const::BOSS_SKILL_THROW_ENEMY_COMPONENT_NAME);
        return throw_skill;
    }
}  // namespace Meow_Meow