#include <defined.h>
#include <meow_meow/component/character_collision_component.h>
#include <meow_meow/const.h>
#include <meow_meow/data/boss_collision_data.h>
#include <meow_meow/data/enemy_collision_data.h>
#include <meow_meow/data/global_data.h>
#include <meow_meow/utils.h>
#include <utils.h>

namespace Meow_Meow {

    Character_Collision_Component::Character_Collision_Component() {}

    Character_Collision_Component::~Character_Collision_Component() {}

    float Character_Collision_Component::get_damage_deal() {
        return this->damage_deal;
    }

    glm::vec2 Character_Collision_Component::get_enemy_direction() {
        return this->enemy_direction;
    }

    void Character_Collision_Component::handle_task(Base_Node* target, float delta_time, void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const Character_Behavior_Config& behavior_config = data->get_config().get_character_behavior_config();

        Character_Node* character = cast_charater_target(target);

        Character_State_Machine_Component* state_machine = get_character_state_machine(target);

        if (state_machine == nullptr) {
            return;
        }

        if (state_machine->is_character_dead()) {
            return;
        }

        /**
         * @Note: if player already hitted or invisible we ignore phase attack damge of enemy!
         */
        if (state_machine->is_character_invincible() || state_machine->is_character_attacked() ||
            state_machine->is_character_flight()) {
            return;
        }

        /**
         * Dashing will be ignore damage!
         */
        if (state_machine->is_character_dashing()) {
            return;
        }

        std::vector<Collision_Information> collisions = this->get_collisioneds();
        for (Collision_Information& collision : collisions) {
            if (collision.tag == Const::BOSS_COLLISION_TAG) {
                Boss_Collision_Data* boss_collison = reinterpret_cast<Boss_Collision_Data*>(collision.owner_data);
                if (boss_collison->get_using_skill_id() != "" && boss_collison->get_skill_damage() > 0) {
                    if (data->get_config().is_boss_flash_skill(boss_collison->get_using_skill_id())) {
                        const Boss_Skill_Flash_Config& skill_config = data->get_config().get_boss_skill_flash_config();
                        state_machine->change_state_at(
                            Const::TRACK_CONTROLL,
                            Const::STATE_FLIGHT,
                            skill_config.fly_duration,
                            Const::CHARACTER_FLIGHT_FROM_BOSS_SKILL_FLASH
                        );
                        boss_collison->set_using_skill_id("");
                        boss_collison->set_skill_damage(0.f);
                    }
                    break;
                }
            }
            if (collision.tag == Const::ENEMY_COLLISION_TAG) {
                Enemy_Collision_Data* enemy_collision = reinterpret_cast<Enemy_Collision_Data*>(collision.owner_data);
                if (enemy_collision->get_using_skill_id() != "" && enemy_collision->get_skill_damage() > 0) {
                    const Boss_Skill_Throw_Enemy_Config& skill_config =
                        data->get_config().get_boss_skill_throw_enemy_config();
                    if (data->get_config().is_boss_throwing_enemy_skill(enemy_collision->get_using_skill_id()) &&
                        collision.distance <= skill_config.distance_hit_collision) {
                        state_machine->change_state_at(
                            Const::TRACK_CONTROLL, Const::STATE_IDLE, State_Machine_Component::INFITY_STATE
                        );
                        state_machine->change_state_at(
                            Const::TRACK_EFFECTED,
                            Const::STATE_STUN,
                            skill_config.duration_stun,
                            Const::CHARACTER_STUN_FROM_BOSS_SKILL_THROW_ENEMY
                        );
                    }
                    enemy_collision->set_using_skill_id("");
                    enemy_collision->set_skill_damage(0.f);
                    break;
                }
            }
            if (collision.tag == Const::BOSS_COLLISION_TAG || collision.tag == Const::ENEMY_COLLISION_TAG) {
                Enemy_Collision_Data* enemy_collision = reinterpret_cast<Enemy_Collision_Data*>(collision.owner_data);
                if (enemy_collision->get_damage_deal() <= 0) {
                    continue;
                }
                this->damage_deal = enemy_collision->get_damage_deal();
                this->enemy_direction = enemy_collision->get_enemy_direction();
                state_machine->change_state_at(
                    Const::TRACK_EFFECTED,
                    Const::STATE_ATTACKED,
                    behavior_config.get_attacked_duration(),
                    Const::CHARACTER_ATTACKED_FROM_EMEMY
                );
                state_machine->change_state_at(
                    Const::TRACK_CONTROLL, Const::STATE_IDLE, State_Machine_Component::INFITY_STATE
                );
                enemy_collision->set_damage_deal(0.f);
                break;
            }
        }
    }
}  // namespace Meow_Meow