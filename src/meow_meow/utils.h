#pragma once
#include <image_node.h>
#include <meow_meow/component/boss_skill_throw_enemy_component.h>
#include <meow_meow/component/boss_state_michine_component.h>
#include <meow_meow/component/character_key_input_component.h>
#include <meow_meow/component/character_skill_dash_component.h>
#include <meow_meow/component/character_skill_thunder_component.h>
#include <meow_meow/component/character_state_machine_component.h>
#include <meow_meow/component/enemy_behavior_component.h>
#include <meow_meow/component/enemy_state_machine_component.h>
#include <meow_meow/const.h>
#include <meow_meow/data/global_data.h>
#include <meow_meow/object/character_node.h>
#include <meow_meow/object/enemy_node.h>
#include <utils.h>

namespace Meow_Meow {
    void fix_background_to_scene(Image_Node* bg, Custom::Size screen_size);

    void run_action_idle_logo(Node* logo, glm::vec2 origin_scale);

    Character_Node* cast_charater_target(Base_Node* target);

    Character_State_Machine_Component* get_character_state_machine(Base_Node* target);

    Character_Skill_Thunder_Component* get_skill_thunder_component(Base_Node* target);

    Character_Skill_Dash_Component* get_skill_dash_component(Base_Node* target);

    Enemy_Node* cast_enemy_target(Base_Node* target);

    Enemy_State_Machine_Component* get_enemy_state_machine_component(Base_Node* target);

    Enemy_Behavior_Component* get_enemy_bihavior_component(Base_Node* target);

    Boss_Node* cast_boss_target(Base_Node* target);

    Boss_State_Machine_Component* get_boss_state_machine_component(Base_Node* target);

    Boss_Skill_Throw_Enemy_Component* get_skill_throw_enemy_component(void* global_data, int boss_id);
};  // namespace Meow_Meow