#include <meow_meow/component/enemy_collision_component.h>
#include <meow_meow/data/bullet_collision_data.h>
#include <meow_meow/data/global_data.h>
#include <meow_meow/utils.h>

namespace Meow_Meow {
    Enemy_Collision_Component::Enemy_Collision_Component() {}

    Enemy_Collision_Component::~Enemy_Collision_Component() {}

    float Enemy_Collision_Component::get_damage_deal() {
        return this->damage_deal;
    }

    Const::DIRECTION Enemy_Collision_Component::get_attacker_direction() {
        return this->attacker_direction;
    }

    void Enemy_Collision_Component::handle_task(Base_Node* target, float delta_time, void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        Enemy_Node* enemy = cast_enemy_target(target);
        const Enemy_Behavior_Config& behavior_config = enemy->get_behavior_config_from(global_data);

        Enemy_State_Machine_Component* state_machine = get_enemy_state_machine_component(target);
        if (state_machine == nullptr) {
            return;
        }

        if (!state_machine->can_take_bullet_damage(global_data)) {
            return;
        }

        std::vector<Collision_Information> collisions = this->get_collisioneds();
        for (Collision_Information& collision : collisions) {
            if (collision.tag != Const::BULLET_COLLISION_TAG)
                continue;
            Bullet_Collision_Data* collision_data = reinterpret_cast<Bullet_Collision_Data*>(collision.owner_data);
            if (collision_data == nullptr)
                continue;
            if (collision_data->get_damage_deal() <= 0 || collision_data->is_hitted())
                continue;
            this->damage_deal = collision_data->get_damage_deal();
            this->attacker_direction = collision_data->get_bullet_direction();
            if (state_machine->is_enemy_stun()) {
                float stun_remain_duration = state_machine->get_duration_remain_state_at(Const::TRACK_EFFECTED);
                state_machine->change_state_at(
                    Const::TRACK_EFFECTED, Const::STATE_STUN, stun_remain_duration, Const::ENEMY_HITTED_WHEN_STUNED
                );
            } else {
                state_machine->change_state_at(
                    Const::TRACK_EFFECTED,
                    Const::STATE_ATTACKED,
                    behavior_config.get_enemy_attacked_duration(),
                    Const::ENEMY_ATTACKED_FROM_BULLET
                );
            }
            collision_data->set_damage_deal(0.f);
            collision_data->set_hitted(true);
            break;
        }
    }
}  // namespace Meow_Meow