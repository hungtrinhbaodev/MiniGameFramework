#include <collision_component.h>
#include <meow_meow/const.h>

namespace Meow_Meow {
    class Enemy_Collision_Component : public Collision_Component {
    public:
        Enemy_Collision_Component();
        ~Enemy_Collision_Component();

        float get_damage_deal();
        Const::DIRECTION get_attacker_direction();

    protected:
        void handle_task(Base_Node* target, float delta_time, void* global_data) override;

    private:
        float damage_deal = 0.f;
        Const::DIRECTION attacker_direction = Const::DIRECTION::NONE;
    };
}  // namespace Meow_Meow