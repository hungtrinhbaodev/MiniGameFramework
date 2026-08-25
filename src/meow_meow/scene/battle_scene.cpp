#include <meow_meow/data/global_data.h>
#include <meow_meow/scene/battle_scene.h>

namespace Meow_Meow {
    Battle_Scene::Battle_Scene() {
        this->battle_layer = new Battle_Layer();
        this->add_child(this->battle_layer);
        this->track_layer_background(this->battle_layer->get_bg());
    }

    Battle_Scene::~Battle_Scene() {}

    void Battle_Scene::attach(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        data->set_battle_layer(this->battle_layer);
    }
}  // namespace Meow_Meow