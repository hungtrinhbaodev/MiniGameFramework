#include <meow_meow/config/character_behavior_config.h>
#include <meow_meow/data/global_data.h>
#include <meow_meow/data/player_data.h>
#include <meow_meow/scene/battle_scene.h>

namespace Meow_Meow {
    Battle_Scene::Battle_Scene() {
        this->battle_layer = new Battle_Layer();
        this->add_child(this->battle_layer);
        this->track_layer_background(this->battle_layer->get_bg());

        this->stats_layer = new Layer_Stats();
        this->stats_layer->set_position({0.f, 0.f});
        this->add_child(this->stats_layer);
    }

    Battle_Scene::~Battle_Scene() {}

    void Battle_Scene::attach(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        data->set_battle_layer(this->battle_layer);

        const Character_Behavior_Config& behavior_config = data->get_config().get_character_behavior_config();
        Player_Data& player_data = data->get_player_data();
        player_data.set_current_health(behavior_config.get_max_health());
        player_data.set_max_health(behavior_config.get_max_health());
        player_data.set_player_damage(behavior_config.get_damage());
    }
}  // namespace Meow_Meow