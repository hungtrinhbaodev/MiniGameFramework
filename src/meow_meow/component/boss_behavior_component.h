#pragma once
#include <meow_meow/component/enemy_behavior_component.h>

namespace Meow_Meow {
    class Boss_Behavior_Component : public Enemy_Behavior_Component {
    public:
        Boss_Behavior_Component();
        ~Boss_Behavior_Component();

    protected:
        const Enemy_Behavior_Config& get_behavior_config(void* global_data) const override;
    };
}  // namespace Meow_Meow