#include <meow_meow/component/boss_behavior_component.h>
#include <meow_meow/data/global_data.h>

namespace Meow_Meow {
    Boss_Behavior_Component::Boss_Behavior_Component() {}

    Boss_Behavior_Component::~Boss_Behavior_Component() {}

    const Enemy_Behavior_Config& Boss_Behavior_Component::get_behavior_config(void* global_data) const {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        return data->get_config().get_boss_behavior_config();
    }

}  // namespace Meow_Meow